import net.jemzart.jsonkraken.JsonKraken
import net.jemzart.jsonkraken.JsonObject
import net.jemzart.jsonkraken.JsonValue
import net.jemzart.jsonkraken.JsonNull

class BinaryTree<T : CustomTypeInterface> {
    private var root: Node<T>? = null

    fun getParentNode(): Node<T>? {
        return root
    }

    fun insert(value: T) {
        root = insertNode(root, value)
    }

    private fun insertNode(current: Node<T>?, value: T): Node<T> {
        if (current == null) {
            return Node(value)
        }
        if (value.compare(current.value) < 0) {
            current.left = insertNode(current.left, value)
        } else {
            current.right = insertNode(current.right, value)
        }
        return current
    }

    fun deleteByValue(value: T) {
        root = deleteNode(root, value)
    }

    private fun deleteNode(current: Node<T>?, value: T): Node<T>? {
        if (current == null) return null

        if (value.compare(current.value) < 0) {
            current.left = deleteNode(current.left, value)
        } else if (value.compare(current.value) > 0) {
            current.right = deleteNode(current.right, value)
        } else {
            if (current.left == null) return current.right
            if (current.right == null) return current.left

            val minValue = findMinValue(current.right!!)
            current.value = minValue
            current.right = deleteNode(current.right, minValue)
        }
        return current
    }

    private fun findMinValue(current: Node<T>): T {
        return current.left?.let { findMinValue(it) } ?: current.value
    }

    fun prettyPrint() {
        println("\n--Binary Tree --")
        printNode(root, "")
    }

    private fun printNode(current: Node<T>?, indent: String) {
        current?.let {
            printNode(it.left, "$indent    ")
            println("$indent${it.value}")
            printNode(it.right, "$indent    ")
        }
    }

    fun rebalance() {
        val values = inOrderTraversal(root).toList()
        root = buildBalancedTree(values)
    }

    private fun inOrderTraversal(current: Node<T>?): Sequence<T> = sequence {
        if (current == null) return@sequence
        yieldAll(inOrderTraversal(current.left))
        yield(current.value)
        yieldAll(inOrderTraversal(current.right))
    }

    private fun buildBalancedTree(values: List<T>): Node<T>? {
        if (values.isEmpty()) return null
        val mid = values.size / 2
        val node = Node(values[mid])
        node.left = buildBalancedTree(values.take(mid))
        node.right = buildBalancedTree(values.drop(mid + 1))
        return node
    }

    fun findByValue(value: T): Int {
        return findByValueNode(root, value, 0)
    }

    private fun findByValueNode(current: Node<T>?, value: T, currentIndex: Int): Int {
        if (current == null) return -1

        val leftSubtreeCount = countNodes(current.left)
        return when {
            value.compare(current.value) == 0 -> currentIndex + leftSubtreeCount
            value.compare(current.value) < 0 -> findByValueNode(current.left, value, currentIndex)
            else -> findByValueNode(current.right, value, currentIndex + leftSubtreeCount + 1)
        }
    }

    private fun countNodes(current: Node<T>?): Int {
        if (current == null) return 0
        return 1 + countNodes(current.left) + countNodes(current.right)
    }

    fun forEach(action: (T) -> Unit) {
        forEachNode(root, action)
    }

    private fun forEachNode(current: Node<T>?, action: (T) -> Unit) {
        current?.let {
            forEachNode(it.left, action)
            action(it.value)
            forEachNode(it.right, action)
        }
    }

    fun serialize(): String {
        return """{"type": "${getType()}", "root": ${serializeNode(root)}}"""
    }

    private fun serializeNode(node: Node<T>?): String {
        return node?.let {
            """{"value": "${it.value.serializeToString()}", "left": ${serializeNode(it.left)}, "right": ${serializeNode(it.right)}}"""
        } ?: "null"
    }

    private fun getType(): String {
        return root?.value?.typeName ?: ""
    }

    private inner class DepthFirstIterator(private val rootNode: Node<T>?) : Iterator<Pair<T, String>> {
        private val stack: MutableList<Pair<Node<T>?, String>> = mutableListOf()
        private var nextValue: Pair<T, String>? = null

        init {
            stack.add(Pair(rootNode, "value"))
        }

        override fun hasNext(): Boolean {
            if (nextValue == null) {
                findNext()
            }
            return nextValue != null
        }

        override fun next(): Pair<T, String> {
            if (hasNext()) {
                val value = nextValue!!
                nextValue = null
                return value
            } else {
                throw NoSuchElementException("No more elements to iterate")
            }
        }

        private fun findNext() {
            while (stack.isNotEmpty() && nextValue == null) {
                val (currentNode, direction) = stack.removeAt(stack.size - 1)

                currentNode?.let {
                    it.right?.let { rightNode -> stack.add(Pair(rightNode, "right")) }
                    it.left?.let { leftNode -> stack.add(Pair(leftNode, "left")) }
                    nextValue = Pair(it.value, direction)
                }
            }
        }
    }



    companion object {
        fun <T : CustomTypeInterface> fromList(values: List<T>): BinaryTree<T> {
            val tree = BinaryTree<T>()
            values.forEach { tree.insert(it) }
            return tree
        }

        fun <T : CustomTypeInterface> fromJsonString(jsonString: String, factory: CustomTypeInterface): BinaryTree<T> {
            val tree = BinaryTree<T>()
            val parsed = JsonKraken.deserialize(jsonString) as JsonObject
            val rootNode = constructTree(parsed["root"], factory) as Node<T>?
            tree.root = rootNode
            return tree
        }

        private fun <T : CustomTypeInterface> constructTree(node: JsonValue, factory: T): Node<T>? {
            if (node is JsonNull) {
                return null
            } else {
                val nodeObj = node as JsonObject
                val value = factory.deserializeFromString(nodeObj["value"].cast<String>()) as T? ?: return null
                val left = constructTree(nodeObj["left"], factory)
                val right = constructTree(nodeObj["right"], factory)
                return Node(value, left, right)
            }
        }
    }
}