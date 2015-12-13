#include "stackVM.h"

stackVM_t initEnv(uint16 stackSize)
{
    stackVM_t env;
    env.istack = list_init_s(stackSize);
    env.varmap = hashmap_init();
    return env;
}

uint32 invokeOp(stackVM_t* env, int opcodes[], bool debug)
{
    int opIndex = 0;
    int currentOp = NOP;

    string vidmem = (string) 0xb8000;
    char oldmem[strlen(vidmem)];
    strcpy(oldmem, vidmem);
    drawFrame(blue, 0, 0, sw, sh - 1);

    while(true)
    {
        currentOp = opcodes[opIndex++];
        switch(currentOp)
        {
        case NOP:
            if(debug)
            {
                messageBox("No operation!");
            }
            continue;
        case EOS:
            messageBox("Done!!!");
            strcpy(vidmem, oldmem);
            return EXEC_SUCCESS;
        case pushi:
        {
            int param1 = opcodes[opIndex++];
            list_addi(&(env->istack), param1);
            if(debug)
            {
                messageBox("Pushed int");
                messageBox(itos10(list_taili(env->istack)));
            }
            break;
        }
        case pushd:
        {
            string param1 = (string) kmalloc(39 * sizeof(char));
            strcat(param1, itos10(opcodes[opIndex++]));
            strcat(param1, ".");
            strcat(param1, itos10(opcodes[opIndex++]));
            list_addf(&(env->istack), (float) stod(param1));
            if(debug)
            {
                messageBox("Pushed double");
                messageBox(ftos(list_tailf(env->istack)));
            }
            break;
        }
        case swap:
        {
            element_t tail = env->istack.data[env->istack.size - 1];
            element_t tmp = env->istack.data[env->istack.size - 2];
            env->istack.data[env->istack.size - 2] = tail;
            env->istack.data[env->istack.size - 1] = tmp;
            if(debug)
            {
                messageBox("Swapped");
            }
            break;
        }
        default:
            if(debug)
            {
                messageBox("Illegal opcode");
                messageBox(itos10(currentOp));
            }
            strcpy(vidmem, oldmem);
            return 1; // 1 means unknown operand
        }
        printAt("First Value", black, 2, 4);
        element_t tmp;
        for(uint32 stacki = 0 ; stacki < env->istack.size; stacki++) 
        {
            paintLine(black, 2, stacki + 5, sw - 1);
            tmp = env->istack.data[stacki];
            switch(tmp.ctype) {
            case INT:
                printAt("i", black, 2, stacki + 5);
                printAt(itos10(etoi(tmp)), black, 4, stacki + 5);
                break;
            case FLT:
                printAt("f", black, 2, stacki + 5);
                printAt(ftos(etof(tmp)), black, 4, stacki + 5);
                break;
            default: break;
            }
        }
    }
}

