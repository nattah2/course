#!/usr/bin/env python3

"""
This program renders a tree that is encoded using Huffman coding.
Huffman coding is a technique to compress data.
At runtime, the program will receive inputs for values to place into the binary tree.

The first line should be a list of symbols.
The second line should be a list of frequencies of those symbols.
"""

import heapq
from collections import defaultdict
import graphviz

class HuffmanNode:
    def __init__(self, symbol=None, frequency=None):
        self.symbol = symbol
        self.frequency = frequency
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.frequency < other.frequency


def build_huffman_tree(freq_table):
    heap = [HuffmanNode(symbol, freq) for symbol, freq in freq_table.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        left_node = heapq.heappop(heap)
        right_node = heapq.heappop(heap)

        internal_node = HuffmanNode(frequency=left_node.frequency + right_node.frequency)
        internal_node.left = left_node
        internal_node.right = right_node

        heapq.heappush(heap, internal_node)

    return heap[0]


def generate_huffman_codes(root, code="", huffman_codes=None):
    if huffman_codes is None:
        huffman_codes = {}

    if root is not None:
        if root.symbol is not None:
            huffman_codes[root.symbol] = code
        generate_huffman_codes(root.left, code + "0", huffman_codes)
        generate_huffman_codes(root.right, code + "1", huffman_codes)

    return huffman_codes

def print_huffman_tree(root, graph):
    global k
    if root is not None:
        if (root.symbol is None):
            # print(str(k), str(root.frequency))
            graph.node(str(k), str(root.frequency))
            name = str(k)
            k += 1
        else:
            # print(root.symbol, root.frequency)
            graph.node(root.symbol, root.symbol + "|" + str(root.frequency))
            name = root.symbol
        x = print_huffman_tree(root.left, dot)
        y = print_huffman_tree(root.right, dot)
        if (x is not None):
            graph.edge(name, x)
        if (y is not None):
            graph.edge(name, y)
        return name


def print_frequency_table(freq_table):
    print("Frequency Table:")
    for symbol, frequency in freq_table.items():
        print(f"Symbol: {symbol}, Frequency: {frequency}")


# Example usage
k = 0

if __name__ == "__main__":
    # example for testing:
    #symbols = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J']
    #frequencies = [12, 3, 9, 6, 13, 4, 6, 9, 1, 17]
    symbols = [x for x in input().split()]
    frequencies = [int(x) for x in input().split()]

    if (len(symbols) != len(frequencies)):
        print("Error! Unequal lengths")
        exit()
    freq_table = dict(zip(symbols, frequencies))

    dot = graphviz.Digraph('Graph')
    dot.attr('node', shape='record')

    huffman_tree = build_huffman_tree(freq_table)
    huffman_codes = generate_huffman_codes(huffman_tree)

    print_huffman_tree(huffman_tree, dot)
    print_frequency_table(freq_table)

    print("\nHuffman Codes:")
    for symbol, code in huffman_codes.items():
        print(f"Symbol: {symbol}, Huffman Code: {code}")


    dot.render(view=True)

