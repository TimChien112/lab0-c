#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#define LESS_THAN(a, b) strcmp(a, b) < 0  // strnatcmp(a, b) < 0

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->size = 0;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {    // check q is empty
        return;  // end this function
    }
    list_ele_t *tmp;           // 1. create a buffer
    while (q->head != NULL) {  // remove elements from the head of the queue
        tmp = q->head->next;   // 2. save the second element's pointer to buffer
        free(q->head->value);  // 3. free the first element
        free(q->head);
        q->head = tmp;  // 4. move the head pointer of queue to the second
                        // element (buffer)
    }
    free(tmp);  // 5. free the buffer memory
    free(q);    // 6. free the queue memory after all of the element are freed
    return;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newe;
    /* 1. Create a new element strcture and assign a memory space for it*/
    newe = malloc(sizeof(list_ele_t));
    if (!newe)
        return false;
    newe->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newe->value) {  // return false if malloc return NULL (malloc failed)
        free(newe);
        return false;
    }
    strncpy(
        newe->value, s,
        strlen(s) + 1);  // 2. copy s content to the pointer of the new element
    newe->next = q->head;  // 3. point the next pointer of the new element to
                           // the perivous head of queue
    q->head =
        newe;   // 4. let the pointer of the queue head point to the new element
    q->size++;  // 5. increase the size of queue
    if (!q->tail) {  // if queue tail is empty
        q->tail = q->head;
        q->tail->next = NULL;  // 6 the head is the tail, the next pointer of
                               // the new element should point to NULL
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newe;
    newe = malloc(sizeof(list_ele_t));
    /* 1. Create a new element strcture and assign a memory space for it*/
    if (!newe)
        return false;
    newe->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newe->value) {  // return false if malloc return NULL (malloc failed)
        free(newe);
        return false;
    }
    strncpy(
        newe->value, s,
        strlen(s) + 1);  // 2. copy s content to the pointer of the new element
    if (!q->size) {      // if queue is empty
        q->head = newe;  // 3.1 the tail element is the head element as well
    } else {
        q->tail->next = newe;  // 3.2 point the next pointer of the original
                               // tail element to the new element
    }
    q->tail =
        newe;   // 4. let the pointer of the queue tail point to the new element
    q->size++;  // 5. increase the size of queue
    q->tail->next =
        NULL;  // 6. point the next pointer of the new element to NULL

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0) {
        return false;
    }
    if (sp) {
        size_t buf_cpy_size =
            bufsize > strlen(q->head->value) ? strlen(q->head->value) : bufsize;
        memset(sp, '\0', buf_cpy_size);
        strncpy(sp, q->head->value, buf_cpy_size - 1);
    }
    list_ele_t *tmp;
    tmp = q->head;
    q->head = q->head->next;
    q->size--;
    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    list_ele_t *tmp, *tmp_tail, *tmp_head;  // create buffers
    tmp_tail = q->tail;                     // 1.
    tmp_head = q->head;                     // 2.
    /* start reversing queue, repeating 3 ~ 6 steps until head reach tmp_tail*/
    do {
        tmp = q->head->next;  // 3.move tmp pointer to the next pointer of the
                              // next element
        q->head->next = q->tail;  // 4.reverse element next pointer
        q->tail = q->head;        // 5.move tail pointer to the head
        q->head = tmp;            // 6.move head pointer to next element
    } while (tmp != tmp_tail);
    q->head->next = q->tail;  // 7.reverse the pointer of head->next
    q->tail =
        tmp_head;  // 8.move queue tail pointer to the original head pointer
    q->tail->next = NULL;  // 9.point the next pointer of tail to NULL
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_merge_sort(queue_t *left_q, queue_t *right_q, queue_t *q)
{
    q->size = left_q->size + right_q->size;
    list_ele_t *tmp_e = NULL;
    left_q->tail->next = tmp_e;
    right_q->tail->next = tmp_e;
    size_t q_cur_size = 0;
    if (LESS_THAN(left_q->head->value, right_q->head->value)) {
        q->head = left_q->head;
        left_q->head = left_q->head->next;
    } else {
        q->head = right_q->head;
        right_q->head = right_q->head->next;
    }
    tmp_e = q->head;
    q_cur_size++;
    while (left_q->head != NULL && right_q->head != NULL) {
        // move head pointer
        if (LESS_THAN(left_q->head->value, right_q->head->value)) {
            tmp_e->next = left_q->head;
            left_q->head = left_q->head->next;
        } else {
            tmp_e->next = right_q->head;
            right_q->head = right_q->head->next;
        }
        tmp_e = tmp_e->next;
        q_cur_size++;  // test q->head->next->value
    }
    tmp_e->next = (left_q->head != NULL) ? left_q->head : right_q->head;

    while (q_cur_size < q->size) {
        tmp_e = tmp_e->next;
        q_cur_size++;
    }
    q->tail = tmp_e;
}
void q_sort(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    /*start half-cutting queue to left_queue & right_queue*/
    queue_t left_q;
    queue_t right_q;
    left_q.size = (q->size >> 1) + (q->size & 1);  // measure the cut point
                                                   // index
    right_q.size = (q->size >> 1);
    left_q.head = q->head;        // assign left_q's head pointer
    right_q.tail = q->tail;       // assign right_q's tail pointer
    list_ele_t *tmp_e = q->head;  // a new element for pointer moving
    for (size_t i = 0; i < left_q.size - 1;
         i++)  // move pointer to the cut point
        tmp_e = tmp_e->next;
    left_q.tail = tmp_e;
    right_q.head = tmp_e->next;
    // finish first half-cut
    q_sort(&left_q);
    q_sort(&right_q);
    /*start merging and sorting*/
    q_merge_sort(&left_q, &right_q, q);
    left_q.head = NULL;
}
