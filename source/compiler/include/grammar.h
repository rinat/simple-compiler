/*
* This code is under GNU Lesser General Public License.
* Simple C Compiler to Low Level Virtual Machine representation
*
* Authors: Rinat Shaikhutdinov (aka Sagit) - architecture, compiler core
*          Alexei Zhdanov (aka Alex) - parser
*          Timofey Tatarinov (aka Tikers) - code generator
*
* University Mari State University
* Faculty Informatics and Computer Science
* Course 4
* Specialization PS-41
* Subject Theory of programming languages and translation methods
* Yoshkar-Ola 2010
*/
#ifndef PARSER_TABLE_P_H
#define PARSER_TABLE_P_H

/*
    grammar representation
*/

namespace compiler
{
    class Grammar
    {
    public:
        enum
        {
            EOF_SYMBOL = 0,
            Token_Add = 26,
            Token_Assign = 27,
            Token_CharConstant = 37,
            Token_Comma = 25,
            Token_Div = 24,
            Token_Equals = 19,
            Token_GT = 16,
            Token_GTE = 17,
            Token_Id = 22,
            Token_LBrace = 7,
            Token_LBracket = 5,
            Token_LParen = 3,
            Token_LT = 20,
            Token_LTE = 21,
            Token_Mult = 23,
            Token_NE = 18,
            Token_NumberConstant = 1,
            Token_RBrace = 8,
            Token_RBracket = 6,
            Token_RParen = 4,
            Token_Semicolon = 9,
            Token_StringConstant = 2,
            Token_Sub = 28,
            Token_break = 11,
            Token_char = 10,
            Token_do = 32,
            Token_double = 14,
            Token_else = 30,
            Token_float = 12,
            Token_for = 31,
            Token_if = 29,
            Token_int = 13,
            Token_main = 15,
            Token_read = 34,
            Token_void = 36,
            Token_while = 33,
            Token_write = 35,

            ACCEPT_STATE = 127,
            RULE_COUNT = 70,
            STATE_COUNT = 129,
            TERMINAL_COUNT = 38,
            NON_TERMINAL_COUNT = 36,

            GOTO_INDEX_OFFSET = 129,
            GOTO_INFO_OFFSET = 155,
            GOTO_CHECK_OFFSET = 155
        };

        static const char  *const spell [];
        static const int            lhs [];
        static const int            rhs [];

        static const int   goto_default [];
        static const int action_default [];
        static const int   action_index [];
        static const int    action_info [];
        static const int   action_check [];

        static inline int nt_action (int state, int nt)
        {
            const int *const goto_index = &action_index [GOTO_INDEX_OFFSET];
            const int *const goto_check = &action_check [GOTO_CHECK_OFFSET];

            const int yyn = goto_index [state] + nt;

            if (yyn < 0 || goto_check [yyn] != nt)
                return goto_default [nt];

            const int *const goto_info = &action_info [GOTO_INFO_OFFSET];
            return goto_info [yyn];
        }

        static inline int t_action (int state, int token)
        {
            const int yyn = action_index [state] + token;

            if (yyn < 0 || action_check [yyn] != token)
                return - action_default [state];

            return action_info [yyn];
        }
    };
}


#endif // PARSER_TABLE_P_H

