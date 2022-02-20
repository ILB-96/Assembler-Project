#include "assembler.h"


int command_code_pross(plw *prv_IC,char *line)/*TODO nead to get the number line for erorrs*/
{
    
    char *operators_array[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int i;
    int ok = 0;
    int operator = -1;
    char command[MAX_LINE] = "";
    strcat(command,line);
    char **s_line = split_line(command);
    for(i = 0; i < SUM_OPERATIONS; i++)
    {
        if(strcmp(s_line[0],operators_array[i]) == 0)
        {  
            operator = i;
            ok = 1;
        }
    }
            switch (operator)
            {
            case 0:/*mov*/
                ok = add_parameters(prv_IC, s_line+1,o_mov, f_mov, two_op);
                break;

            case 1:/*cmp*/
                ok = add_parameters(prv_IC, s_line+1,o_cmp, f_cmp, full_two_op);
                break;
            case 2:/*add*/
                ok = add_parameters(prv_IC, s_line+1,o_add, f_add, two_op);
                break;
            case 3:/*sub*/
                ok = add_parameters(prv_IC, s_line+1,o_sub, f_sub, two_op);
                break;
            case 4:/*lea*/
                ok = add_parameters(prv_IC, s_line+1,o_lea, f_lea, min_two_op);
                break;
            case 5:/*clr*/
                ok = add_parameters(prv_IC, s_line+1,o_clr, f_clr, target_op);
                break;
            case 6:/*not*/
                ok = add_parameters(prv_IC, s_line+1,o_not, f_not, target_op);
                break;
            case 7:/*inc*/
                ok = add_parameters(prv_IC, s_line+1,o_inc, f_inc, target_op);
                break;
            case 8:/*dec*/
                ok = add_parameters(prv_IC, s_line+1,o_dec, f_dec, target_op);
                break;
            case 9:/*jmp*/
                ok = add_parameters(prv_IC, s_line+1,o_jmp, f_jmp, min_target_op);
                break;
            case 10:/*bne*/
                ok = add_parameters(prv_IC, s_line+1,o_bne, f_bne, min_target_op);
                break;
            case 11:/*jsr*/
                ok = add_parameters(prv_IC, s_line+1,o_jsr, f_jsr, min_target_op);
                break;
            case 12:/*red*/
                ok = add_parameters(prv_IC, s_line+1,o_red, f_red, target_op);
                break;
            case 13:/*prn*/
                ok = add_parameters(prv_IC, s_line+1,o_prn, f_prn, full_target_op);
                break;
            case 14:/*rts*/
                ok = add_parameters(prv_IC, s_line+1,rts, 0, no_op);
                break;
            case 15:/*stop*/
                ok = add_parameters(prv_IC, s_line+1,stop, 0, no_op);
                break;
            default:
                ok = 0;
                printf("illigle command in \"%s\"-",*s_line);
                break;
            }
    
    return ok;
}




int add_parameters(plw *prv,char **comm, opcode opcod, Funct funct,Valid_operator op)
{
    registers source_r = 0, target_r = 0;
    sortType source_sort = 0, target_sort = 0;
    ARE are = 0;
    int ok = 0;

    /*1) get the size of element*/
    int i = 0, size = 0;
    addBase_word(prv,A,opcod);
    
    while(comm[i] != NULL)
    {
        i++;
        size++;
         if(strcmp(comm[i-1],"") == 0)size--;
    }
    
    if(size == 3 && *comm[1] == ',')/*case two operators*/
    {
        ok = setSort_and_register(comm[0],&source_r,&source_sort,&are);
        ok = setSort_and_register(comm[2],&target_r,&target_sort,&are);

        addStd_word(prv,are,funct,source_r,source_sort,target_r,target_sort);
        ok = add_word_by_source(prv, comm[0], source_sort, op);
        ok = add_word_by_target(prv, comm[2], target_sort, op);

    }
    else if(size == 1)/*case one operator*/
    {
        ok = setSort_and_register(comm[0],&target_r,&target_sort,&are);
        
        ok = addStd_word(prv,are,funct,source_r,source_sort,target_r,target_sort);
        ok = add_word_by_target(prv, comm[0], target_sort, op);
    }
    else if(size != 0)
    {
        
         printf("\n-illigal command \"%s\"-",*comm);/*TODO need to print the number line*/
         ok = 0;
    }

    return ok; 
}     
int add_word_by_source(plw *prv, char* comm, sortType source_sort, Valid_operator op)
{
        int ok  = 1;
        switch (source_sort)
        {
        case immediate:
            if(op == full_two_op || op == two_op)
            {
                add_numTo_list(prv,A, atoi(comm+1));
            }
            else
            {
                ok = 0;
                printf("\n-illigal operetion-\"%s\"-",comm);/*TODO need to print the number line*/
            }
            break;
        case direct: 
        case index_sort:
                addTo_list(prv,0);
                addTo_list(prv,0);
        case register_direct:
            if(op == min_two_op && source_sort == register_direct)/*why the 'case' don't do it's work???*/
            {
                ok = 0;
                printf("\n-illigal operetion-\"%s\"-",comm);/*TODO need to print the number line*/
            }
            break;
        }
    return ok;
}


int add_word_by_target(plw *prv, char* comm, sortType target_sort, Valid_operator op)
{
    int ok = 1;
    
        switch (target_sort)
        {
        case immediate:
            if(op == full_two_op || op == full_target_op)
            {
                add_numTo_list(prv,A, atoi(comm+1));
            }
            else
            {
                ok = 0;
                printf("\n-illigal operetion-\"%s\"-",comm);/*TODO need to print the number line*/
            }
            break;
        case direct: 
        case index_sort:
            addTo_list(prv,0);
            addTo_list(prv,0);
            break;
        }
        return ok;
}
        

int setSort_and_register(char *operator, registers *r, sortType *sort, ARE *are)
{
    int result = 1;
    *are = A;
    /*case it's register*/
    if(operator[0] == 'r')
    {
        int temp = atoi(operator+1);
        if(temp >= r0 && temp <= r15)
        { 
            *r = temp;
        }else result = 0;
        *sort = register_direct;
    }
    /*case it's number*/
    else if(operator[0] == '#')
    {
         *r = r0;
         *sort = immediate;
    }
    /*case it's variable*/
    else
    {
        /*case illigal variable*/
        if(isdigit(operator[0])) result = 0;
        else
        {
            int parenthesis = 0;
            int reg;
            int i =0;
            while(operator[i] != '\0')
            {
                if(parenthesis == 1 && operator[i] == 'r' && (reg = atoi(operator+i+1)) <= r15 &&  reg >= r0)
                {
                   /*TODO check if it's letter after the number like [r3A]*/
                        
                   *r = reg;
                   *sort = index_sort;
                  
                   parenthesis++;
                }
                if(operator[i] == '[' ||operator[i] == ']') parenthesis++;
                i++;
            }
            if(!parenthesis)
            {
                *r = r0;
                *sort = direct;
            }
            else if(parenthesis != 3) result = 0;
        }
    }

    return result;
}




char ** split_line(char *str)
{
    char **s = malloc(sizeof(char*));
    int i = 0;
    char *token;
    
    token = strtok(str, " ");
    while( token != NULL ) 
    {
        char* temp;
        char* c;
        int j = 0;
        while(token[j] != '\0')
        {
        while (token[j] != ',' && token[j] != '\0' && token[j] != '\n')j++;
        if(token[j] == ',' )
        {
            
            temp = token;
            token[j] = '\0';
            c = ",";
            token = token+j+1;
            if(temp[0] != '\0')
            {
                s[i++] = temp;
                s = (char **)realloc(s, sizeof(char*) * i+1);
            }
            s[i++] = c;
            s = (char **)realloc(s, sizeof(char*) * i+1);
           
            j = 0;
        }
        else
        {
            if(token[j] == '\n') token[j] = '\0';
            s[i++] = token;
            s = (char **)realloc(s, sizeof(char*) * i+1);
        }
           
        }
       token = strtok(NULL, " ");
    }

    s[i] = NULL; 
    return s;
}




/*
int main()
{
   
    plw head;
    plw prv;
    initialize_list(&head,&prv,BASE_STOCK);
    
    
    char s1[] ={"add    r3,LIST"};
    char s2[] ={"prn    #48"};
    char s3[] ={"lea STR,r6"};
    char s4[] ={"inc r6"};
    
   

    
    
    twoParam_1(&prv, split_line(s1)+1,o_add,f_add,two_op);
    twoParam_1(&prv, split_line(s2)+1,o_prn,f_prn,full_target_op);
    twoParam_1(&prv, split_line(s3)+1,o_lea,f_lea,min_two_op);
    twoParam_1(&prv, split_line(s4)+1,o_inc,f_inc,target_op);
    
    
    
    command_code_pross(s1);
    
    
   
    return 0;
}

*/

