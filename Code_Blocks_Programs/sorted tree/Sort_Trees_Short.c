//Elier Bermudez, COP 3502, Midterm Exam 2
//Write a function to add a child to a parent, but maintain the property that the children
//of any given parent are sorted by payload value in increasing order.
void add_sorted_child(tree_node *parent, tree_node *child)
{
    parent->nchildren++;
    //If the list is empty, add the child in
    if(parent->child == NULL)
    {
        parent->child = child;
    }
    else
        //If the new child is less than the first child, simply place it on the left.
        if(parent->child->payload > child->payload)
        {
            child->sibling = parent->child;
            parent->child = child;
            return;
        }
    //If not, look for the first child that our new child is less than
        else
        {
            tree_node *current_child = parent->child->sibling;
            tree_node *previous_child = parent->child;
            //Continue as long as our current child has a sibling
            //This is a do while to prevent it from breaking when the number of children is 2
            do
            {
                if(current_child->payload > child->payload)
                {
                    //Set the previous_child's sibling to be the new child
                    previous_child->sibling = child;
                    //Set the current child's sibling to be the current_child
                    child->sibling = current_child;
                    return;
                }
                else
                {
                    previous_child = current_child;
                    current_child = current_child->sibling;
                }
            }
            while(current_child->sibling!=NULL);
            //If the new child was not less than anything else, we place it at the end
            previous_child->sibling = child;
        }
}
