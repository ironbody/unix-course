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
        printf("\tpushq\t%d\n", p->con.value); // push
        break;
    case typeId:
        printf("\tpushq\t%c\n", p->id.i + 'a'); // TODO push index of symbol array
        break;
    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("\tcmpq\t$0,\t%%rax\n");
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjz\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2)
            {
                /* if else */
                printf("\tcmpq\t$0\t%%rax\n");
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
                printf("\tcmpq\t$0\t%%rax\n");
                printf("\tjz\tL%03d\n", lbl1 = lbl++); // jz
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:
            ex(p->opr.op[0]);
            // pop in i någgot reg rsi
            printf("\tpopq\t%%rsi\n");
            printf("\tmovq\t$string,\t%%rdi\n");
            printf("\tcall\tprintf\n");
            break;
        case '=':
            ex(p->opr.op[1]);
            printf("\tpopq\t%c\n", p->opr.op[0]->id.i + 'a'); // pop
            break;
        case UMINUS:
            ex(p->opr.op[0]);
            printf("\tnegq\tD\n"); // negative
            break;
        case FACT:
            ex(p->opr.op[0]);
            // printf("\tfact\n"); // todo
            printf("\tfact\n");
            break;
        case LNTWO:
            ex(p->opr.op[0]);
            printf("\nsar\t$1,\tD\n"); // todo
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch (p->opr.oper)
            {
            case GCD:
                printf("\tgcd\n"); // todo
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
                printf("\timull\t%%r11, %%r10\n");
                printf("\tpushq\t%%r10\n");
                // printf("\timull\tS, D\n"); // results in %rdx:%rax
                break;
            case '/':
                printf("\tpopq\t%%r11\n");
                printf("\tpopq\t%%rax\n");
                printf("\tidivq\t%%r11\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '<':
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovl\t$0,\t%%rax\n");  // flytta 0 i rax ifall comp är true
                // printf("\tcmovne\tS,\tD\n");
                // printf("\tcompLT\n");
                break;
            case '>':
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovg\t$0,\t%%rax\n");  // flytta 0 i rax ifall comp är true
                // printf("\tcompGT\n");
                break;
            case GE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovge\t$0,\t%%rax\n"); // flytta 0 i rax ifall comp är true
                // printf("\tcompGE\n");
                break;
            case LE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovle\t$0,\t%%rax\n"); // flytta 0 i rax ifall comp är true
                // printf("\tcompLE\n");
                break;
            case NE:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmovne\t$0,\t%%rax\n"); // flytta 0 i rax ifall comp är true
                printf("\tpop\t%%r10\n");
                // printf("\tcompNE\n");
                break;
            case EQ:
                printf("\tpopq\t%%r10\n"); // sätt senaste variablerna i r10 och r11
                printf("\tpopq\t%%r11\n");
                printf("\tcmpq\t%%r11\t%%r10\n"); // compare
                printf("\tmovq\t$1,\t%%rax\n");   // flytta 1 i rax så att rax != 0 ifall comp är false
                printf("\tcmove\t$0,\t%%rax\n");  // flytta 0 i rax ifall comp är true
                // printf("\tcompEQ\n");
                break;
            }
        }
    }
    return 0;
}
