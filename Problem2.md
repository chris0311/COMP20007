Part A

```pseudocode
A <-- Array
count <-- 0
function inorder_traversal(tree):
// Traverse the tree in order and put values in an array
	if (tree != NULL):
		if (tree->val1 != NULL):
            inorder_traversal(tree->child0)
            A[count] = tree->val1
            count <-- count + 1
			inorder_traversal(tree->child1)
			
		 if (tree->val2 != NULL):
         	inorder_traversal(tree->child2)
            A[count] = tree->val2
            count <-- count + 1
            
		 if (tree->val3 != NULL):
         	inorder_traversal(tree->child4)
            A[count] = tree->val3
            count <-- count + 1

function find_median(A[0...n]):
	return A[(n-1)/2]
```

Part B

```pseudocode
function count_descendants(node):
	num_descendant <-- 0
	for not null child in node->childrens:
		num_decendant <-- num_descendant + child->num_vals + child->num_descendant
	return num_descendant
```

Part C

```pseudocode
// Get total count of value in the tree
total_val_count <-- tree->num_descendants + tree->num_vals
target_count <-- (total_val_count + 1) / 2
temp = 0

function find_median_efficient(tree):
	if (tree->child0 != NULL):
		temp = tree->child0->num_descendants + tree->child0->num_vals // Number of values in the first node
	if (total_val_count - temp < target_count):
		return find_median_efficient(tree->child0)
	else if (total_val_count - temp == target_count):
		return tree->val0
	total_val_count = total_val_count - temp - 1
	temp = 0
	
	if (tree->child1 != NULL):
		temp = tree->child1->num_descendants + tree->child1->num_vals // Number of values in the second node
	else if (total_val_count - temp < target_count):
		return find_median_efficient(tree->child1)
	else if (total_val_count - temp == target_count):
		return tree->val1
	total_val_count = total_val_count - temp - 1
	temp = 0
	
	if (tree->child2 != NULL):
		temp = tree->child2->num_descendants + tree->child2->num_vals // Number of values in the third node
	else if (total_val_count - temp < target_count):
		return find_median_efficient(tree->child2)
	else if (total_val_count - temp == target_count):
		return tree->val2
	total_val_count = total_val_count - temp - 1
	temp = 0
	
	// Median must be in the last child
	return find_median_efficient(tree->child3)
	
```

Part D

Since `num_descendants` and `num_vals` are included in the tree structure, it will only take $O(1)$ time to get them. 

In the best case, in which the median is the first value of the root, it will only take constant time to get the result.

In the worst case, the median is in the leaf node of the tree, which will take $\theta(h)$ time to travel to the leaf node of the tree, where $h$ represents the height of the tree. 

