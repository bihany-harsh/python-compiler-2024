class Node:
    def __init__(self, data, color="red"):
        self.data = data
        self.color = color
        self.parent = None
        self.left = None
        self.right = None

class RedBlackTree:
    def __init__(self):
        self.NIL = Node(data=None, color="black")  # Sentinel node
        self.root = self.NIL

    def insert(self, data):
        # Insertion code here (not included for brevity)
        pass

    def delete(self, node):
        if node == self.NIL:
            return

        original_color = node.color
        if node.left == self.NIL:
            temp = node.right
            self.transplant(node, node.right)
        elif node.right == self.NIL:
            temp = node.left
            self.transplant(node, node.left)
        else:
            successor = self.minimum(node.right)
            original_color = successor.color
            temp = successor.right
            if successor.parent == node:
                temp.parent = successor
            else:
                self.transplant(successor, successor.right)
                successor.right = node.right
                successor.right.parent = successor
            self.transplant(node, successor)
            successor.left = node.left
            successor.left.parent = successor
            successor.color = node.color

        if original_color == "black":
            self.delete_fixup(temp)

    def transplant(self, u, v):
        if u.parent == self.NIL:
            self.root = v
        elif u == u.parent.left:
            u.parent.left = v
        else:
            u.parent.right = v
        v.parent = u.parent

    def delete_fixup(self, node):
        while node != self.root and node.color == "black":
            if node == node.parent.left:
                sibling = node.parent.right
                if sibling.color == "red":
                    sibling.color = "black"
                    node.parent.color = "red"
                    self.left_rotate(node.parent)
                    sibling = node.parent.right
                if sibling.left.color == "black" and sibling.right.color == "black":
                    sibling.color = "red"
                    node = node.parent
                else:
                    if sibling.right.color == "black":
                        sibling.left.color = "black"
                        sibling.color = "red"
                        self.right_rotate(sibling)
                        sibling = node.parent.right
                    sibling.color = node.parent.color
                    node.parent.color = "black"
                    sibling.right.color = "black"
                    self.left_rotate(node.parent)
                    node = self.root
            else:
                # Mirror code for the right child
                pass
        node.color = "black"

    def left_rotate(self, node):
        # Left rotation code here (not included for brevity)
        pass

    def right_rotate(self, node):
        # Right rotation code here (not included for brevity)
        pass

    def minimum(self, node):
        while node.left != self.NIL:
            node = node.left
        return node