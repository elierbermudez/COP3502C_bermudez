bool BinarySearchTree::remove(int value)
{
    //If there is no root, return false
    if (root == NULL){
        return false;
    }

    else{
        //If the root is what we have to remove, then
        if (root->getValue() == value){
            //Make an extra node called auxRoot
            BSTNode auxRoot(0);

            //Set auxRoot's left child to be the root
            auxRoot.setLeftChild(root);

            //Use an extra node to store what gets removed when we pass
            //The value to be removed and the parent of root (which would be auxRoot)
            BSTNode* removedNode = root->remove(value, &auxRoot);

            //Set the root to be the left child of auxroot for some reason???
            root = auxRoot.getLeft();

            if (removedNode != NULL){
                delete removedNode;
                return true;
            }
            else{
                return false;
            }
        }
        else{
            //If we aren't removing the root, try to remove from the root given just the value and not the parent
            BSTNode* removedNode = root->remove(value, NULL);

            if (removedNode != NULL){
                delete removedNode;
                return true;
            }
            else{
                return false;
            }
        }
    }
}

BSTNode* BSTNode::remove(int value, BSTNode *parent)
{
    //If the input value is less than the value of the node we are on
    if (value < this->value){
        //And this is NOT the leftmost root
        if (left != NULL){
            //Then continue downwards trying to remove using this node as the parent
            return left->remove(value, this);
        }
        else{
            //If the input value is less but this is the leftmost node then the input value can't be in our tree.
            return NULL;
        }
    }
    //If the input value is MORE than the value of the node we are on
    else if (value > this->value){
        //And this is NOT the rightmost root
        if (right != NULL){
            //Then continue downwards trying to remove using this node as the parent
            return right->remove(value, this);
        }
        else{
            //If the input value is greater but this is the rightmost node then the input value can't be in our tree.
            return NULL;
        }
    }
    //If the input value is neither greater than nor less than our current value, that means the node we are on is the one to be removed
    else{
        //If our node has both a left child and a right child
        if (left != NULL && right != NULL){
            //Set this node's value to be the leftmost right descendent's value
            //EPIPHANY
            //NODES DON'T ACTUALLY HAVE TO BE SWAPPED
            //YOU JUST HAVE TO SWAP THEIR VALUES
            //IT'S JUST A NAME AND A COUNT THERE'S NO REASON TO BOTHER MESSING AROUND WITH MOVING A BUNCH OF POINTERS EVERYWERE
            this->value = right->minValue();

            //delete and return the entry on the right side with the same value as the one we have now
            //Look at the tree deletion example we saw before
            //When doing it this way, instead of "swapping" the 7 and 8, we instead turn the 7 into an 8,
            //then look for another 8 (that being the original one) and remove it
            return right->remove(this->value, this);
        }
        //If our node has either 1 child or 0 children, and is the left child of it's parent...
        else if (parent->left == this){
            //Thing = condition ? result_if_true: result_if_false;
            //...then if our node's left child is not null, we take that left child and make it the new left child of the parent.
            //Otherwise, if our node's left child IS null, we take the right child and make it the new left child of the parent
            //Essentially all this is doing is figuring out which child to set as the parent's left or right
            parent->left = (left != NULL) ? left : right;
            return this;
        }
        else if (parent->right == this){
            //This does the same as above except it is updating the parent's right child
            //If our node ends up having 0 children, then both left and right will be NULL,
            //So we can feel free to just assign parent->right to be NULL since the thing that was deleted
            //Was a leaf
            parent->right = (left != NULL) ? left : right;
            return this;
        }
    }
}

int BSTNode::minValue()
//Finds the leftmost right descendent
{
    //If you can't go left anymore, return the value
    if (left == NULL){
        return value;
    }
    //If you can still go left, keep going
    else{
        return left->minValue();
    }
}
