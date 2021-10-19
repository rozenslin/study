#include <iostream>
#include <cstdlib>
using namespace std;
typedef struct Node Node;
// node for single-linked list.
struct Node
{
    char* uname;
    int   uid;
    Node* next;
};

void ShowList(Node** list)
{
    if (!list)
        return;
    for (Node* node = *list; node != NULL; node = node->next)
    {
        cout << "User with name: " << node->uname << " id: " << node->uid << endl;
    }
}
 
// always add into the head
bool UpdateList(Node** list, bool add, const char* name, const int id)
{
    bool success = false;
    if (!list || !name)
        return success;

    if (add)
    {
        // create a new node, make it the head of list
        // XXX no dup check
        Node* node = new Node();
        node->uid = id;
        node->uname = strdup(name);

        node->next = *list;
        *list = node;
        cout << "User with name: " << node->uname << " id: " << node->uid << " added" <<endl;
        fprintf(stderr,"ahooooo:adding %s:%d---node: %p, name: %p\n",__FUNCTION__,__LINE__, (void*)node, (void*)node->uname);

        success = true;
    }
    else
    {
        // search and find the node, then del it, if not found, do nothing.
        /* way 1:
        {
            Node* prev = NULL;
            Node* cur = *list;
            while(cur)
            {
                if (!strcmp(name, cur->uname))
                {
                    // recon the list
                    if (prev)
                        prev->next = cur->next;
                    else
                        *list = cur->next;

                    // free the node
                    fprintf(stderr,"ahooooo:freed %s:%d---node: %p, name: %p\n",__FUNCTION__,__LINE__, (void*)(cur), (void*)(cur)->uname);
                    cout << "User with name: " << cur->uname << " id: " << cur->uid << " deleted" <<endl;
                    free(cur->uname);
                    delete cur;

                    break;
                }
                else
                {
                    prev = cur;
                    cur = cur->next;
                }
            }
        } */

        /* way 2: */
        {
            
            Node** p = list;
            while (*p)
            {
                if (!strcmp(name, (*p)->uname))
                {
                    // save current position
                    Node* t = *p;

                    // free the node
                    fprintf(stderr,"ahooooo:freed %s:%d---node: %p, name: %p\n",__FUNCTION__,__LINE__, (void*)(*p), (void*)(*p)->uname);
                    cout << "User with name: " << (*p)->uname << " id: " << (*p)->uid << " deleted" <<endl;
                    free((*p)->uname);

                    // ahooooooooooooo
                    // recon the list
                    *p = (*p)->next;

                    delete t;
                    break;
                }
                else
                {
                    // wahooooooooooooo
                    p = &((*p)->next);
                }
            }
        
        }

        success = true;
    }

    return success;
}

int main()
{
    // the list head
    struct Node* list = NULL;
    bool input_on = true;
    string op, name, uid;
    int id;

    while (input_on)
    {
        cout << "Input the operation: add/delete/over/list:";
        cin >> op;
        if (op == "over")
        {
            cout << "Task over, exiting..." << endl;
            break;
        }
        else if (op == "add")
        {
            cout << "Input username for add:";
            cin >> name;
            if (name == "")
            {
                cout << "Invalid username for add, giving up this operation..." << endl;
                continue;
            }
            cout << "Input uid for add:";
            cin >> uid;
            if (uid == "" || !(id = atoi(uid.c_str())))
            {
                cout << "Invalid uid for add, giving up this operation..." << endl;
                continue;
            }
            UpdateList(&list, true, name.c_str(), id);
            if (!list)
            {
                cout << "Failed to add, exiting..." << endl;
                break;
            }
        }
        else if (op == "delete")
        {
            cout << "Input username for delete:";
            cin >> name;
            if (name == "")
            {
                cout << "Invalid username for delete, giving up this operation..." << endl;
                continue;
            }
            UpdateList(&list, false, name.c_str(), 0);
        }
        else if (op == "list")
        {
            ShowList(&list);
            continue;
        }
        else 
        {
            cout << "Invalid operation, plz input add/delete/over:" << endl;
            continue;
        }
    }

    return 0;
}
