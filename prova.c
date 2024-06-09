#include <stdio.h>
#include <stdlib.h>

// ciao come va
// Definizione della struttura del nodo
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// Funzione per aggiungere un nuovo nodo alla fine della lista
void append(Node **head_ref, int new_data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *last = *head_ref;

    new_node->data = new_data;
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = new_node;
}

// Funzione per stampare la lista
void printList(Node *node)
{
    while (node != NULL)
    {
        printf("%d -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

// Funzione per aggiornare i dati della lista
void updateList(Node *node, int new_value)
{
    while (node != NULL)
    {
        node->data = new_value;
        node = node->next;
        new_value++;
    }
}

int main()
{
    Node *head = NULL;

    // Aggiunta di alcuni dati alla lista
    append(&head, 1);
    append(&head, 2);
    append(&head, 3);

    // Stampa iniziale della lista
    printf("Lista iniziale: ");
    printList(head);

    // Aggiornamento dei dati della lista
    updateList(head, 10);

    // Stampa della lista aggiornata
    printf("Lista aggiornata: ");
    printList(head);

    // Libera la memoria allocata per la lista
    Node *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }

    return 0;
}
