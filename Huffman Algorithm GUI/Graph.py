#!/usr/bin/env python3

"""
This program renders a tree given by the user at runtime.
At runtime, the program will receive inputs for values to place into the binary tree.
If a -1 is received, the program will exit and render using GraphViz.
"""

import os

class TreeNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None


def insert(root, key):
    if root is None:
        return TreeNode(key)
    else:
        if root.key < key:
            root.right = insert(root.right, key)
        else:
            root.left = insert(root.left, key)
    return root


def generate_dot_file(root, filename):
    dot_content = "digraph BST {\n"
    dot_content += "  node [shape=circle];\n"
    dot_content = generate_dot_content(root, dot_content)
    dot_content += "}\n"

    with open(filename, 'w') as dot_file:
        dot_file.write(dot_content)


def generate_dot_content(root, dot_content):
    if root is not None:
        if root.left is not None:
            dot_content += f'  {root.key} -> {root.left.key};\n'
            dot_content = generate_dot_content(root.left, dot_content)
        if root.right is not None:
            dot_content += f'  {root.key} -> {root.right.key};\n'
            dot_content = generate_dot_content(root.right, dot_content)
    return dot_content


if __name__ == "__main__":
    root = None
    f = 0
    while (f != -1):
        f = input("")
        root = insert(root, f)
        dot_filename = "bst_graphviz.dot"
        generate_dot_file(root, dot_filename)
        os.system("dot -Tpng bst_graphviz.dot -o bst_graphviz.png")
        os.system("xdg-open bst_graphviz.png")
