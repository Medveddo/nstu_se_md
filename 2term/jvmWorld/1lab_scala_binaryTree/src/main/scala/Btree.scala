import scala.collection.immutable.Seq

class Node[T](var value: T, var left: Option[Node[T]], var right: Option[Node[T]])

class BinaryTree[T <: CustomTypeTrait] {
  private var root: Option[Node[T]] = None
  def insert(value: T): Unit = {
    root = insertNode(root, value)
  }

  private def insertNode(current: Option[Node[T]], value: T): Option[Node[T]] = current match {
    case None => Some(new Node(value, None, None))
    case Some(node) =>
      if (value.compare(node.value) < 0) {
        node.left = insertNode(node.left, value)
      } else {
        node.right = insertNode(node.right, value)
      }
      Some(node)
  }

  def deleteByValue(value: T): Unit = {
    root = deleteNode(root, value)
  }

  private def deleteNode(current: Option[Node[T]], value: T): Option[Node[T]] = current match {
    case None => None
    case Some(node) =>
      if (value.compare(node.value) < 0) {
        node.left = deleteNode(node.left, value)
      } else if (value.compare(node.value) > 0) {
        node.right = deleteNode(node.right, value)
      } else {
        (node.left, node.right) match {
          case (None, _) => return node.right
          case (_, None) => return node.left
          case _ =>
            val minValue = findMinValue(node.right)
            node.value = minValue
            node.right = deleteNode(node.right, minValue)
        }
      }
      Some(node)
  }

  private def findMinValue(current: Option[Node[T]]): T = current match {
    case Some(node) =>
      node.left match {
        case None => node.value
        case Some(_) => findMinValue(node.left)
      }
    case None => throw new NoSuchElementException("Empty tree")
  }

  def prettyPrint(): Unit = {
    println(s"\n--Binary Tree --")
    printNode(root, "")
  }

  def getParentNode(): Option[Node[T]] = root

  private def printNode(current: Option[Node[T]], indent: String): Unit = current match {
    case Some(node) =>
      printNode(node.left, indent + "    ")
      println(indent + node.value)
      printNode(node.right, indent + "    ")
    case None =>
  }

  def deleteByIndex(index: Int): Unit = {
    root = deleteNodeByIndex(root, index)
  }

  private def deleteNodeByIndex(current: Option[Node[T]], index: Int): Option[Node[T]] = current match {
    case None => None
    case Some(node) =>
      val count = countNodes(node.left)
      if (index < count) {
        node.left = deleteNodeByIndex(node.left, index)
      } else if (index > count) {
        node.right = deleteNodeByIndex(node.right, index - count - 1)
      } else {
        (node.left, node.right) match {
          case (None, _) => return node.right
          case (_, None) => return node.left
          case _ =>
            val minValue = findMinValue(node.right)
            node.value = minValue
            node.right = deleteNodeByIndex(node.right, 0)
        }
      }
      Some(node)
  }

  private def countNodes(current: Option[Node[T]]): Int = current match {
    case Some(node) =>
      1 + countNodes(node.left) + countNodes(node.right)
    case None => 0
  }

  def rebalance(): Unit = {
    val values = inOrderTraversal(root)
    root = buildBalancedTree(values)
  }

  private def inOrderTraversal(current: Option[Node[T]]): List[T] = current match {
    case Some(node) =>
      inOrderTraversal(node.left) ++ List(node.value) ++ inOrderTraversal(node.right)
    case None => Nil
  }

  private def buildBalancedTree(values: List[T]): Option[Node[T]] = values match {
    case Nil => None
    case _ =>
      val mid = values.length / 2
      val node = new Node(values(mid), None, None)
      node.left = buildBalancedTree(values.take(mid))
      node.right = buildBalancedTree(values.drop(mid + 1))
      Some(node)
  }

  def findByValue(value: T): Int = {
    findByValueNode(root, value, 0)
  }

  def forEach(f: T => Unit): Unit = {
    forEachNode(root, f)
  }

  private def forEachNode(current: Option[Node[T]], f: T => Unit): Unit = current match {
    case Some(node) =>
      forEachNode(node.left, f)
      f(node.value)
      forEachNode(node.right, f)
    case None =>
  }

  private def findByValueNode(current: Option[Node[T]], value: T, currentIndex: Int): Int = current match {
    case Some(node) =>
      val leftSubtreeCount = countNodes(node.left)
      if (value.compare(node.value) == 0) {
        currentIndex + leftSubtreeCount
      } else if (value.compare(node.value) < 0) {
        findByValueNode(node.left, value, currentIndex)
      } else {
        findByValueNode(node.right, value, currentIndex + leftSubtreeCount + 1)
      }
    case None => -1
  }

  def depthFirstIterator: Iterator[(T, String)] = new DepthFirstIterator(root)

  def serialize(): String = {
    s"""{"type": "${getType}", "root": ${serializeNode(root)}}"""
  }

  private def serializeNode(node: Option[Node[T]]): String = node match {
    case Some(n) =>
      s"""{"value": "${n.value.serializeToString}", "left": ${serializeNode(n.left)}, "right": ${serializeNode(n.right)}}"""
    case None => "null"
  }

  private def getType: String = {
    if (root.isDefined) root.get.value.typeName else ""
  }

  private class DepthFirstIterator(private val rootNode: Option[Node[T]]) extends Iterator[(T, String)] {
    private val stack: scala.collection.mutable.Stack[(Option[Node[T]], String)] = scala.collection.mutable.Stack()
    private var nextValue: Option[(T, String)] = None

    stack.push((rootNode, "value"))

    override def hasNext: Boolean = {
      if (nextValue.isEmpty) {
        findNext()
      }
      nextValue.isDefined
    }

    override def next(): (T, String) = {
      if (hasNext) {
        val value = nextValue.get
        nextValue = None
        value
      } else {
        throw new NoSuchElementException("No more elements to iterate")
      }
    }

    private def findNext(): Unit = {
      while (stack.nonEmpty && nextValue.isEmpty) {
        val (currentNode, direction) = stack.pop()

        currentNode match {
          case Some(node) =>
            if (node.right.isDefined) {
              stack.push((node.right, "right"))
            }
            if (node.left.isDefined) {
              stack.push((node.left, "left"))
            }
            nextValue = Some((node.value, direction))
          case None =>
        }
      }
    }
  }
}


object BinaryTree {
  def fromList[T <: CustomTypeTrait](values: Seq[T]): BinaryTree[T] = {
    val tree = new BinaryTree[T]
    values.foreach(tree.insert)
    tree
  }

  def fromJsonString[T <: CustomTypeTrait : Manifest](jsonString: String, factory: CustomTypeTrait): BinaryTree[T] = {
    val tree = new BinaryTree[T]
    val parsed = ujson.read(jsonString)
    val rootNode = constructTree(parsed("root"), factory).asInstanceOf[Option[Node[T]]]
    tree.root = rootNode
    tree
  }

  private def constructTree[T <: CustomTypeTrait : Manifest](node: ujson.Value, factory: CustomTypeTrait): Option[Node[CustomTypeTrait]] = {
    if (node.isNull) {
      None
    } else {
      val value = factory.deserializeFromString(node("value").str).get
      val left = constructTree(node("left"), factory)
      val right = constructTree(node("right"), factory)
      Some(new Node[CustomTypeTrait](value, left, right))
    }
  }
}
