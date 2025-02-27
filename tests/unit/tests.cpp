#include <gtest/gtest.h>
#include <limits.h>

extern "C" {
#include "../../src/calculator.h"
}

// Тесты для NumStack
TEST(NumStackTest, InitAndEmpty)
{
    NumStack s;
    init_num(&s);
    EXPECT_TRUE(is_empty_num(&s));
}

TEST(NumStackTest, PushAndPop)
{
    NumStack s;
    init_num(&s);
    Number num;
    num.int_val = 42;
    push_num(&s, num);
    EXPECT_FALSE(is_empty_num(&s));

    Number popped = pop_num(&s);
    EXPECT_EQ(popped.int_val, 42);
    EXPECT_TRUE(is_empty_num(&s));
}

// Тесты для OpStack
TEST(OpStackTest, InitAndEmpty)
{
    OpStack s;
    init_op(&s);
    EXPECT_TRUE(is_empty_op(&s));
}

TEST(OpStackTest, PushPopPeek)
{
    OpStack s;
    init_op(&s);
    push_op(&s, '+');
    EXPECT_FALSE(is_empty_op(&s));
    EXPECT_EQ(peek_op(&s), '+');

    EXPECT_EQ(pop_op(&s), '+');
    EXPECT_TRUE(is_empty_op(&s));
}

// Тесты для priority
TEST(PriorityTest, Operators)
{
    EXPECT_EQ(priority('+'), 1);
    EXPECT_EQ(priority('-'), 1);
    EXPECT_EQ(priority('*'), 2);
    EXPECT_EQ(priority('/'), 2);
    EXPECT_EQ(priority('('), 0);
    EXPECT_EQ(priority('a'), 0);
}

// Тесты для apply_operation
class ApplyOperationTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        a.int_val = 10;
        b.int_val = 2;
        a_f.float_val = 10.5;
        b_f.float_val = 2.5;
    }

    Number a, b, res;
    Number a_f, b_f;
};

TEST_F(ApplyOperationTest, IntOperations)
{
    flag_float = 0;

    res = apply_operation(a, b, '+');
    EXPECT_EQ(res.int_val, 12);

    res = apply_operation(a, b, '-');
    EXPECT_EQ(res.int_val, 8);

    res = apply_operation(a, b, '*');
    EXPECT_EQ(res.int_val, 20);

    res = apply_operation(a, b, '/');
    EXPECT_EQ(res.int_val, 5);
}

TEST_F(ApplyOperationTest, FloatOperations)
{
    flag_float = 1;

    res = apply_operation(a_f, b_f, '+');
    EXPECT_DOUBLE_EQ(res.float_val, 13.0);

    res = apply_operation(a_f, b_f, '-');
    EXPECT_DOUBLE_EQ(res.float_val, 8.0);

    res = apply_operation(a_f, b_f, '*');
    EXPECT_DOUBLE_EQ(res.float_val, 26.25);

    res = apply_operation(a_f, b_f, '/');
    EXPECT_DOUBLE_EQ(res.float_val, 4.2);
}

TEST_F(ApplyOperationTest, DivisionByZero)
{
    b.int_val = 0;
    flag_float = 0;
    EXPECT_EXIT(apply_operation(a, b, '/'), ::testing::ExitedWithCode(1), "");

    b_f.float_val = 0.0;
    flag_float = 1;
    EXPECT_EXIT(apply_operation(a_f, b_f, '/'), ::testing::ExitedWithCode(1), "");
}

// Тесты для validate_input
TEST(ValidateInputTest, ValidExpressions)
{
    EXPECT_TRUE(validate_input("2+3*4"));
    EXPECT_TRUE(validate_input("(2+3)*4"));
    EXPECT_TRUE(validate_input(" 123 "));
    EXPECT_TRUE(validate_input("((3+5)/2)"));
}

TEST(ValidateInputTest, InvalidExpressions)
{
    EXPECT_FALSE(validate_input("2 + * 3"));
    EXPECT_FALSE(validate_input("(2+3"));
    EXPECT_FALSE(validate_input("2a+3"));
    EXPECT_FALSE(validate_input("2 3"));
    EXPECT_FALSE(validate_input("+2+3"));
    EXPECT_FALSE(validate_input("2++3"));
    EXPECT_FALSE(validate_input("2+(3*4))"));
}

// Тесты для evaluate
class EvaluateTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Сброс флага перед каждым тестом
        flag_float = 0;
    }
};

TEST_F(EvaluateTest, BasicIntOperations)
{
    Number res = evaluate("3+4*2");
    EXPECT_EQ(res.int_val, 11);

    res = evaluate("(3+4)*2");
    EXPECT_EQ(res.int_val, 14);

    res = evaluate("10/3");
    EXPECT_EQ(res.int_val, 3);
}

TEST_F(EvaluateTest, IntEdgeCases)
{
    EXPECT_EXIT(evaluate("10/0"), ::testing::ExitedWithCode(1), "");
}
