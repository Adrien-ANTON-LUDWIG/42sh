#include "lexer_redir.h"

#include "custom_descriptor.h"
#include "lexer.h"
#include "lexer_utils.h"
#include "tokens.h"

struct token *lexer_redir(struct major *mj, struct token *tk, char *word)
{
    if (word && *word >= '0' && *word <= '9')
        list_append(mj, tk->data, &word[0]);
     
    free(word);
    return tk;
}

int is_redir(char *word)
{
    char *str[] = TOKENS_STRINGS_REDIR;
    int is_redir = 0;
    char *s = word;
    if (s && *s >= '0' && *s <= '9')
        s++;
        
    for (unsigned long i = 0; i < sizeof(str) / sizeof(char *); i++)
    {
        if (strcmp(s, str[i]) == 0)
        {
            is_redir = 1;
            break;
        }
    }
    return is_redir;
}

int next_is_redirection(struct major *mj)
{
    size_t temp = mj->file->lexer_index;
    
    int return_value = 0;

    char *s = get_redir(mj);

    if (!s)
        return 0;

    return_value = is_redir(s);
    free(s);
    
    mj->file->lexer_index = temp;
    return return_value;
}