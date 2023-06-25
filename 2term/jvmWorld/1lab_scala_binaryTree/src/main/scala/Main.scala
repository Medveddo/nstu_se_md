import scalafx.Includes._
import scalafx.application.JFXApp3
import scalafx.collections.ObservableBuffer
import scalafx.scene.Scene
import scalafx.scene.control.{Button, ComboBox, TreeItem, TreeView}
import scalafx.scene.layout.VBox

import scala.math.BigDecimal.RoundingMode


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
        val l = List()

        if (rightNode == null && leftNode == null) {

          //          rootNode.children = List(leftNode, rightNode)
          //          rootNode.expanded = true
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
    //    val tree = BinaryTree.fromList(Seq(
    //      new Point(42.36, -15.06),
    //      new Point(37.77, -10.42),
    //      new Point(51.51, -26.13)
    //    ))(new PointDistanceComparator)
    //    val tree = BinaryTree.fromList(Seq())(new DefaultComparator)


    // how to move BinaryTree[_] outside GUI layer ???
    // SecretTurboLayer that accept:
    // 1 - TYPE
    // 2 - has actions
    //     -- add element from string
    //     -- rebalances tree
    var tree: BinaryTree[_] = BinaryTree.fromList(Seq.empty[Int])(new DefaultComparator[Int])
    var gselectedOption: String = "-"
    tree.prettyPrint()

    // get options from builder


    var treeView: TreeView[String] = null
    val btree_root_node = tree.getParentNode()

    treeView = new TreeView[String](getTreeItemForBinaryTree(tree.getParentNode()))
    treeView.showRoot = true

    val updateButton = new Button("Update")
    val rebalanceButton = new Button("Rebalance")
    updateButton.onAction = _ => {
      // Perform the tree update here
      val r = scala.util.Random
      if (gselectedOption == "-") {
        tree.asInstanceOf[BinaryTree[Int]].insert(
          r.nextInt()
          //        new Point(
          //        BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble,
          //        BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
          //        //        r.nextDouble() * 200 - 100,
          //        //        r.nextDouble() * 200 - 100
          //      )
        )
      }
      else if (gselectedOption == "2DPoint") {
        tree.asInstanceOf[BinaryTree[Point]].insert(
          new Point(
            BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble,
            BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
          )
        )
      }

      else if (gselectedOption == "GeoCoordinate") {
        tree.asInstanceOf[BinaryTree[GeoCoordinate]].insert(
          new GeoCoordinate(
            BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble,
            BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
          )
        )
      }


      // Get the new root node for the tree
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      tree.prettyPrint()
      // Update the treeView
      treeView.root = newRootNode
    }

    rebalanceButton.onAction = _ => {
      tree.rebalance()
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }

    val options = ObservableBuffer("-", "GeoCoordinate", "2DPoint")
    val select = new ComboBox[String](options)
    select.value = "-"
    select.onAction = () => {
      val selectedOption = select.value.value
      println(s"Selected: $selectedOption")
      gselectedOption = selectedOption
      if (selectedOption == "GeoCoordinate") {

        tree = BinaryTree.fromList(Seq.empty[GeoCoordinate])(new GeoCoordinateLatitudeComparator)
      }
      else if (selectedOption == "2DPoint") {
        tree = BinaryTree.fromList(Seq.empty[Point])(new PointDistanceComparator)
      }
      else if (selectedOption == "-") {
        tree = BinaryTree.fromList(Seq.empty[Int])(new DefaultComparator[Int])
      }
      val newRootNode = getTreeItemForBinaryTree(tree.getParentNode())
      treeView.root = newRootNode
    }

    val container = new VBox()
    container.prefWidth = 600

    // todo try put pretty print to GUI instead of tree view
    // todo save to file button
    // todo load from file
    // todo add element (string -> GeoCord) + (string -> ...)
    // todo sort ???
    container.children = Seq(treeView, updateButton, rebalanceButton, select)

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
