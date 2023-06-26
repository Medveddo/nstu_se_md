import scalafx.Includes._
import scalafx.application.JFXApp3
import scalafx.collections.ObservableBuffer
import scalafx.scene.Scene
import scalafx.scene.control.{Button, ComboBox, TextField, TreeItem, TreeView}
import scalafx.scene.layout.VBox
import scalafx.scene.text.Text


object Main extends JFXApp3 {

  private def getTreeItemForBinaryTree[T](node: Option[Node[T]], direction: String = ""): TreeItem[String] = {
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
    var selectedTypeOption: String = treeProviderOptionTypes.head
    treeProvider.initializeWithEmptyTree(selectedTypeOption)
    var tree: BinaryTree[_] = treeProvider.getTree() match {
      case Some(actualTree) => actualTree
      case None => throw new NoSuchElementException("No BinaryTree available")
    }

    var treeView: TreeView[String] = null
    treeView = new TreeView[String](getTreeItemForBinaryTree(tree.getParentNode()))
    treeView.showRoot = true

    val elementRepresentationExample: Text = new Text(
      treeProvider.getObjectStringRepresentationExample()
    )

    val inputDataField: TextField = new TextField()
    inputDataField.layoutX = 20
    inputDataField.layoutY = 20
    inputDataField.promptText = "Enter data string representation"

    val insertRandomButton = new Button("Insert Random")
    val rebalanceButton = new Button("Rebalance")
    val addElementButton = new Button("Add Element")
    val saveToFileButton = new Button("Save to file")
    val loadFromFileButton = new Button("Load from file")

    insertRandomButton.onAction = _ => {
      treeProvider.insertRandomValue()
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }
    rebalanceButton.onAction = _ => {
      tree.rebalance()
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }
    addElementButton.onAction = _ => {
      val inputValue: String = inputDataField.text.value
      treeProvider.insertValue(selectedTypeOption, inputValue)
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }

    val selectOptions = ObservableBuffer(treeProviderOptionTypes: _*)
    val select = new ComboBox[String](selectOptions)
    select.value = treeProviderOptionTypes.head
    select.onAction = () => {
      selectedTypeOption = select.value.value
      treeProvider.initializeWithEmptyTree(selectedTypeOption)
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
