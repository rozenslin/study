#include <iostream>
#include <cstdlib>
using namespace std;
typedef struct Node Node;
struct Node
{
    Node* prev;
    char* uname;
    int   uid;
    Node* next;
};
// always add into the head
void ShowList(Node** list)
{
    if (!list)
        return;
    for (Node* node = *list; node != NULL; node = node->next)
    {
        cout<<"User with name: " << node->uname << " id: " << node->uid << endl;
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

        node->prev = NULL;
        node->next = *list;
        if (*list)
            (*list)->prev = node;
        *list = node;
        cout<<"User with name: " << node->uname << " id: " << node->uid << " added" <<endl;
        fprintf(stderr,"ahooooo:adding %s:%d---node: %p, name: %p\n",__FUNCTION__,__LINE__, (void*)node, (void*)node->uname);

        success = true;
    }
    else
    {
        // search and find the node, then del it, if not found, do nothing.
        // XXX no dup check
        Node* node;
        for (node = *list; node != NULL; node = node->next)
        {
            if (!strcmp(name, node->uname))
            {
                // recon the list
                if (node->prev)
                {
                    node->prev->next = node->next;
                }
                else
                {
                    // update head
                    *list = node->next;
                }

                if (node->next)
                    node->next->prev = node->prev;

                // free the node
                fprintf(stderr,"ahooooo:freed %s:%d---node: %p, name: %p\n",__FUNCTION__,__LINE__, (void*)node, (void*)node->uname);
                cout<<"User with name: " << node->uname << " id: " << node->uid << " deleted" <<endl;
                free(node->uname);
                delete node;

                break;
            }
        }
        success = true;
    }

    return success;
}

int main()
{
    // create the list head
    struct Node* list = NULL;
    // get input and store into list
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
        // output list
    }

    return 0;
}
