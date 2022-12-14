#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p)
{
    int lbl1, lbl2;

    if (!p)
        return 0;
    switch (p->type)
    {
    case typeCon:
        printf("\tpush\t$%d\n", p->con.value);
        break;
    case typeId:
        printf("\tpush\tsym+%u(%%rip)\n", 8 * p->id.i);
        break;
    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2)
            {
                /* if else */
                // printf("\tcmpq\t$0, %%rax\n");
                printf("\tjz\tL%03d\n", lbl1 = lbl++); // jz
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            }
            else
            {
                /* if */
                // printf("\tcmpq\t$0, %%rax\n");
                printf("\tjz\tL%03d\n", lbl1 = lbl++); // jz
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:
            ex(p->opr.op[0]);
            // pop in i någgot reg rsi
            printf("\tpop\t%%rsi\n");
            printf("\tlea\tstring(%%rip), %%rdi\n");
            printf("\tcall\tprintf\n");
            break;
        case '=':
            ex(p->opr.op[1]);
            //printf("\tpopq\t%c\n", p->opr.op[0]->id.i + 'a'); // pop
            
            printf("\tpop\tsym+%u(%%rip)\n", 8 * p->opr.op[0]->id.i);
            // printf("\tmov\t%%r10, ($sym, $%u, $8)\n", p->opr.op[0]->id.i);
            break;
        case UMINUS:
            ex(p->opr.op[0]);
            printf("\tpop\t%%r9\n"); // negative
            printf("\tnegq\t%%r9\n"); // negative
            printf("\tpush\t%%r9\n"); // negative
            break;
        case FACT:
            ex(p->opr.op[0]);

            printf("\tpop %%rdi\n");
            printf("\tcall fact\n");
            printf("\tpush %%rax\n");
            break;
        case LNTWO:
            ex(p->opr.op[0]);

            printf("\tpop %%rdi\n");
            printf("\tcall lntwo\n"); 
            printf("\tpush %%rax\n");
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch (p->opr.oper)
            {
            case GCD:
                printf("\tpop %%rsi\n");
                printf("\tpop %%rdi\n");
                printf("\tcall gcd\n");
                printf("\tpush %%rax\n");

                break;
            case '+':
                printf("\tpopq\t%%r11\n");
                printf("\tpopq\t%%r10\n");
                printf("\taddq\t%%r11, %%r10\n");
                printf("\tpushq\t%%r10\n");
                break;
            case '-':
                printf("\tpopq\t%%r11\n");
                printf("\tpopq\t%%r10\n");
                printf("\tsubq\t%%r11, %%r10\n");
                printf("\tpushq\t%%r10\n");
                break;
            case '*':
                printf("\tpopq\t%%r11\n");
                printf("\tpopq\t%%r10\n");
                printf("\timulq\t%%r11, %%r10\n");
                printf("\tpushq\t%%r10\n");
                // printf("\timull\tS, D\n"); // results in %rdx:%rax
                break;
            case '/':
                printf("\tpopq\t%%r11\n");
                printf("\tmov\t$0, %%rdx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tidiv\t%%r11\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '<':
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovl\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true
                printf("\tcmpq\t$1, %%r9\n");

                // printf("\tcmovne\tS,\tD\n");
                // printf("\tcompLT\n");
                break;
            case '>':
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovg\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true
                printf("\tcmpq\t$1, %%r9\n");
                // printf("\tcompGT\n");
                break;
            case GE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false

                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovge\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true                
                printf("\tcmpq\t$1, %%r9\n");

                // printf("\tcompGE\n");
                break;
            case LE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovle\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true
                printf("\tcmpq\t$1, %%r9\n");
                // printf("\tcompLE\n");
                break;
            case NE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovne\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true
                printf("\tcmpq\t$1, %%r9\n");
                // printf("\tpop\t%%r10\n");
                // printf("\tcompNE\n");
                break;
            case EQ:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r10, %%r11\n"); // compare
                printf("\tmovq\t$1, %%r9\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tmovq\t$0, %%r10\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmove\t%%r10, %%r9\n");  // flytta 0 i rax ifall comp är true
                printf("\tcmpq\t$1, %%r9\n");
                // printf("\tcompEQ\n");
                break;
            }
        }
    }
    return 0;
}
