/* This file was made incase I changed my mind about the way i want to do certain things*/
/*
typedef struct label
{
    char name[32];
    struct label *next;
    struct label *prev;
} label;

typedef struct list
{
    label *head;
} list;

list *l;
int is_init = 0;
if ((l = malloc(sizeof(*l))) == NULL)
{
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
}
if ((l->head = malloc(sizeof(label))) == NULL)
{
    free(l);
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILUSRE);
}
if (is_init)
    freeLabels(l);
free(l->head);
free(l);

void init(char *label_name, list *l)
{

    strcpy(l->head->name, label_name);
    l->head->next = NULL;
    l->head->prev = NULL;
}
void add(list *l, char *label_name)
{

    label *new_label;
    label *last_label = l->head;
    if ((new_label = malloc(sizeof(*new_label))) == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    while (last_label->next != NULL)
        last_label = last_label->next;

    strcpy(new_label->name, label_name);
    new_label->prev = last_label;
    new_label->next = NULL;

    last_label->next = new_label;
}

void freeLabels(list *l)
{
    while (l->head->next != NULL)
    {
        l->head = l->head->next;
        free(l->head->prev);
    }
}
*/