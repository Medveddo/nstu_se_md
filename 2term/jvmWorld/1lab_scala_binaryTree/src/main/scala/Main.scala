import scalafx.Includes._
import scalafx.application.JFXApp3
import scalafx.collections.ObservableBuffer
import scalafx.scene.Scene
import scalafx.scene.control.{Button, ComboBox, TextField, TreeItem, TreeView}
import scalafx.scene.layout.VBox
import scalafx.scene.text.Text


object ScalaFXBinaryTree extends JFXApp3 {

  def getTreeItemForBinaryTree[T](node: Option[Node[T]], direction: String = ""): TreeItem[String] = {
    var rootNode: TreeItem[String] = null
    var leftNode: TreeItem[String] = null
    var rightNode: TreeItem[String] = null
    node match {
      case Some(existed_node) =>
        rootNode = new TreeItem[String](direction + existed_node.value.toString)
        if (existed_node.right.isDefined) {
          rightNode = getTreeItemForBinaryTree(existed_node.right, "R\t")
        }
        if (existed_node.left.isDefined) {
          leftNode = getTreeItemForBinaryTree(existed_node.left, "L\t")
        }

        if (rightNode == null && leftNode == null) {

        } else if (rightNode != null && leftNode == null) {
          rootNode.children = List(new TreeItem[String]("Empty leaf"), rightNode)
          rootNode.expanded = true
        } else if (rightNode == null && leftNode != null) {
          rootNode.children = List(leftNode, new TreeItem[String]("Empty leaf"))
          rootNode.expanded = true
        } else {
          rootNode.children = List(leftNode, rightNode)
          rootNode.expanded = true
        }
        rootNode
      case None =>
        new TreeItem[String]("Empty Tree")
    }
  }

  override def start(): Unit = {
    val treeProvider = new TreeProvider()
    val treeProviderOptionTypes = treeProvider.getAvailableTypes()

    treeProvider.initializeWithEmptyTree()
    var tree: BinaryTree[_] = treeProvider.getTree() match {
      case Some(actualTree) => actualTree
      case None => throw new NoSuchElementException("No BinaryTree available")
    }
    var gselectedOption: String = treeProviderOptionTypes.head

    tree.prettyPrint()

    var treeView: TreeView[String] = null
    treeView = new TreeView[String](getTreeItemForBinaryTree(tree.getParentNode()))
    treeView.showRoot = true

    var elementRepresentationExample: Text = new Text(
      treeProvider.getObjectStringRepresentationExample()
    )

    val insertRandomButton = new Button("Insert Random")
    val rebalanceButton = new Button("Rebalance")
    val addElementButton = new Button("Add Element")
    val saveToFileButton = new Button("Save to file")
    val loadFromFileButton = new Button("Load from file")

    insertRandomButton.onAction = _ => {
      treeProvider.insertRandomValue(gselectedOption)
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      tree.prettyPrint()
      treeView.root = newRootNode
    }

    val inputDataField: TextField = new TextField()
    inputDataField.layoutX = 20
    inputDataField.layoutY = 20
    inputDataField.promptText = "Enter data string representation"

    rebalanceButton.onAction = _ => {
      tree.rebalance()
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }
    addElementButton.onAction = _ => {
      val inputValue: String = inputDataField.text.value
      treeProvider.insertValue(gselectedOption, inputValue)
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      tree.prettyPrint()
      treeView.root = newRootNode
    }

    val options = ObservableBuffer(treeProviderOptionTypes: _*)
    val select = new ComboBox[String](options)
    select.value = treeProviderOptionTypes.head

    select.onAction = () => {
      val selectedOption = select.value.value
      println(s"Selected: $selectedOption")
      gselectedOption = selectedOption
      treeProvider.initializeWithEmptyTree(gselectedOption)
      tree = treeProvider.getTree() match {
        case Some(actualTree) => actualTree
        case None => throw new NoSuchElementException("No BinaryTree available")
      }

      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode

      elementRepresentationExample.text = treeProvider.getObjectStringRepresentationExample()
    }

    val container = new VBox()
    container.prefWidth = 600



    // todo try put pretty print to GUI instead of tree view
    // todo save to file button
    // todo load from file
    // todo add element (string -> GeoCord) + (string -> ...)
    // todo sort ???
    container.children = Seq(
      treeView,
      insertRandomButton,
      rebalanceButton,
      select,
      elementRepresentationExample,
      inputDataField,
      addElementButton,
    )

    stage = new JFXApp3.PrimaryStage {
      title = "1lab_scala_binary_tree"
      scene = new Scene {
        content = container
      }
      width = 800
      height = 600
    }
  }
}
