#include "calcmath.h"

CalcMath::CalcMath() {}

QString CalcMath::baseConversion(const QString &num, int ori_base,
                                 int new_base) {
    if (num == "") {
        return (QString)(num);
    }
    bool ok = true;
    qlonglong ori_num = num.toLongLong(&ok, ori_base);

    if (!ok) {
        return ("ERROR");
    }
    return QString::number(ori_num, new_base);
}

int CalcMath::binary_calculate(const qlonglong num1, const qlonglong num2,
                            QChar opt) {
    bool ok = true;
    qlonglong numL1 = 0, numL2 = 0, res = 0;

    switch(opt.unicode()) {
    case((ushort)'+'): {
        res = numL1 + numL2;
        break;
    }
    case((ushort)'-'): {
        res = numL1 - numL2;
        break;
    }
    case((ushort)'*'): {
        res = numL1 * numL2;
        break;
    }
    case((ushort)'/'): {
        res = numL1 / numL2;
        break;
    }
    case((ushort)'&'): {
        res = numL1 & numL2;
        break;
    }
    case((ushort)'|'): {
        res = numL1 | numL2;
        break;
    }
    // '!' just to avoid confusion, the role in the calculator is the same as '~'
    case((ushort)'!'):
    case((ushort)'~'): {
        int bitWidth = numL2 == 0 ? 1 : static_cast<int>(std::log2(numL2) + 1);
        int bitMask = (1 << bitWidth) - 1;
        res = (~numL2) & bitMask;
        break;
    }
    case((ushort)'>'): {
        res = numL1 >> numL2;
        break;
    }
    case((ushort)'<'): {
        res = numL1 << numL2;
        break;
    }
    case((ushort)'^'): {
        res = std::pow(numL1, numL2);
        break;
    }
    case((ushort)'%'): {
        res = numL1 % numL2;
        break;
    }
    defualt: {
        return(-1);
    }
    }
    return res;
}

int CalcMath::calculate(const std::string &f) {
    QStack<qlonglong> nums;               // stack to store numbers
    QStack<QChar> ops;              // stack to store operators
    int num = 0;
    char op = '+';                  // start with '+' to handle the first number

    for (size_t i = 0; i <= f.size(); ++i) {
        char c = (i < f.size()) ? f[i] : '\0';  // to handle the last number
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        } else if (c == '(') {
            int j = i, braces = 1;
            while (braces > 0) {
                ++i;
                if (f[i] == '(') ++braces;
                if (f[i] == ')') --braces;
            }
            num = calculate(f.substr(j + 1, i - j - 1));
        } else if (!isdigit(c) || i == f.size()) {
            if (op == '+') {
                nums.push(num);
            } else if (op == '-') {
                nums.push(-num);
            } else {
                qlonglong bin_res = binary_calculate(nums.top(), num, op);
                nums.pop();
                nums.push(bin_res);
            }
            num = 0;
            op = c;
        }
    }

    int res = 0;

    while (!nums.empty()) {
        res += nums.top();
        nums.pop();
    }

    return res;
}
