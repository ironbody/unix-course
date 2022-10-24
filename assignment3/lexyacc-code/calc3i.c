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
                printf("\tje\tL%03d\n", lbl1 = lbl++); // jz
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            }
            else
            {
                /* if */
                printf("\tje\tL%03d\n", lbl1 = lbl++); // jz
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:
            ex(p->opr.op[0]);
            char temp[] = "some variable";
            printf("\t%s\%edi\n", temp); // print
            printf("\tcall\tputs\n");
            break;
        case '=':
            ex(p->opr.op[1]);
            printf("\tpopq\t%c\n", p->opr.op[0]->id.i + 'a'); // pop
            break;
        case UMINUS:
            ex(p->opr.op[0]);
            printf("\tnegq\n"); // negative
            break;
        case FACT:
            ex(p->opr.op[0]);
            printf("\tfact\n"); //
            break;
        case LNTWO:
            ex(p->opr.op[0]);
            printf("\ntwo\n"); // todo
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch (p->opr.oper)
            {
            case GCD:
                printf("\tgcd\n");
                break;
            case '+':
                printf("\tadd\n");
                break;
            case '-':
                printf("\tsub\n");
                break;
            case '*':
                printf("\tmul\n");
                break;
            case '/':
                printf("\tdiv\n");
                break;
            case '<':
                printf("\tcompLT\n");
                break;
            case '>':
                printf("\tcompGT\n");
                break;
            case GE:
                printf("\tcompGE\n");
                break;
            case LE:
                printf("\tcompLE\n");
                break;
            case NE:
                printf("\tcompNE\n");
                break;
            case EQ:
                printf("\tcompEQ\n");
                break;
            }
        }
    }
    return 0;
}
