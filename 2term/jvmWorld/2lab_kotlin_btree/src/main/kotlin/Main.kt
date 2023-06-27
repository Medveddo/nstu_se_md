import javafx.application.Application
import javafx.collections.FXCollections
import javafx.scene.Scene
import javafx.scene.control.*
import javafx.scene.layout.VBox
import javafx.stage.FileChooser
import javafx.stage.Stage
import java.io.File
import java.io.PrintWriter


class BinaryTreeApp : Application() {

    private fun getTreeItemForBinaryTree(node: Node<*>?, direction: String = ""): TreeItem<String> {
        if (node != null) {
            val rootNode = TreeItem<String>("$direction${node.value}")

            val leftNode: TreeItem<String>? = if (node.left != null) {
                getTreeItemForBinaryTree(node.left, "L\t")
            } else {
                null
            }

            val rightNode: TreeItem<String>? = if (node.right != null) {
                getTreeItemForBinaryTree(node.right, "R\t")
            } else {
                null
            }

            when {
                leftNode == null && rightNode == null -> {
                    // No operation
                }
                leftNode != null && rightNode == null -> {
                    rootNode.children.addAll(TreeItem("Empty leaf"), leftNode)
                }
                leftNode == null && rightNode != null -> {
                    rootNode.children.addAll(rightNode, TreeItem("Empty leaf"))
                }
                else -> {
                    rootNode.children.addAll(leftNode, rightNode)
                }
            }

            rootNode.isExpanded = true
            return rootNode
        } else {
            return TreeItem("Empty tree")
        }
    }

    override fun start(primaryStage: Stage) {
        val treeProvider = TreeProvider()
        val treeProviderOptionTypes = treeProvider.getAvailableTypes()
        var selectedTypeOption = treeProviderOptionTypes.first()
        treeProvider.initializeWithEmptyTree(selectedTypeOption)

        var tree = treeProvider.getTree() ?: throw NoSuchElementException("No BinaryTree available")

        var treeView = TreeView<String>(getTreeItemForBinaryTree(tree.getParentNode()))
        treeView.isShowRoot = true

        val elementRepresentationExample = Label(treeProvider.getObjectStringRepresentationExample())

        val inputDataField = TextField()
        inputDataField.promptText = "Enter data string representation"

        val insertRandomButton = Button("Insert Random")
        val rebalanceButton = Button("Rebalance")
        val addElementButton = Button("Add Element")
        val saveToFileButton = Button("Save to file")
        val loadFromFileButton = Button("Load from file")

        insertRandomButton.setOnAction {
            treeProvider.insertRandomValue()
            treeView.root = getTreeItemForBinaryTree(tree.getParentNode())
        }

        rebalanceButton.setOnAction {
            tree.rebalance()
            treeView.root = getTreeItemForBinaryTree(tree.getParentNode())
        }

        addElementButton.setOnAction {
            val inputValue = inputDataField.text
            treeProvider.insertValue(selectedTypeOption, inputValue)
            treeView.root = getTreeItemForBinaryTree(tree.getParentNode())
        }

        val selectOptions = FXCollections.observableArrayList(treeProviderOptionTypes)
        val select = ComboBox<String>(selectOptions)
        select.value = treeProviderOptionTypes.first()
        select.setOnAction {
            selectedTypeOption = select.value
            treeProvider.initializeWithEmptyTree(selectedTypeOption)
            tree = treeProvider.getTree() ?: throw NoSuchElementException("No BinaryTree available")
            treeView.root = getTreeItemForBinaryTree(tree.getParentNode())
            elementRepresentationExample.text = treeProvider.getObjectStringRepresentationExample()
        }

        val fileChooser = FileChooser()
        fileChooser.initialDirectory = File("C:\\Users\\Vitaly\\Workspace\\StudyNSTU\\nstu_se_md\\2term\\jvmWorld\\2lab_kotlin_btree")
        fileChooser.extensionFilters.addAll(
            FileChooser.ExtensionFilter("JSON Files", "*.json"),
            FileChooser.ExtensionFilter("Text Files", "*.txt")
        )

        saveToFileButton.setOnAction {
            val selectedFile = fileChooser.showSaveDialog(primaryStage)
            selectedFile?.let {
                val filePath = it.absolutePath
                val data = treeProvider.getTreeStringRepresentation()
                data?.let { stringData ->
                    PrintWriter(filePath).use { writer ->
                        writer.write(stringData)
                    }
                }
            }
        }

        loadFromFileButton.setOnAction {
            val selectedFile = fileChooser.showOpenDialog(primaryStage)
            selectedFile?.let {
                val filePath = it.absolutePath
                val content = File(filePath).readText()
                selectedTypeOption = treeProvider.setTreeFromStringRepresentation(content)
                tree = treeProvider.getTree() ?: throw NoSuchElementException("No BinaryTree available")
                treeView.root = getTreeItemForBinaryTree(tree.getParentNode())
                elementRepresentationExample.text = treeProvider.getObjectStringRepresentationExample()
                select.value = selectedTypeOption
            }
        }

        val container = VBox()
        container.prefWidth = 600.0
        container.children.addAll(
            treeView,
            insertRandomButton,
            rebalanceButton,
            select,
            elementRepresentationExample,
            inputDataField,
            addElementButton,
            saveToFileButton,
            loadFromFileButton
        )

        primaryStage.title = "2lab_kotlin_btree"
        primaryStage.scene = Scene(container, 800.0, 700.0)
        primaryStage.show()
    }
}

fun main() {
    Application.launch(BinaryTreeApp::class.java)
}
