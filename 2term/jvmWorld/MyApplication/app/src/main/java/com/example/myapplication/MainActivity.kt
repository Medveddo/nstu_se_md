package com.example.myapplication

import android.os.Bundle
import android.view.View
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import androidx.appcompat.app.AppCompatActivity
import android.net.Uri
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import java.io.*

import android.widget.*

class MainActivity : AppCompatActivity() {

    private lateinit var saveToFileButton: Button
    private lateinit var loadFromFileButton: Button
    private lateinit var createFileLauncher: ActivityResultLauncher<String>
    private lateinit var openFileLauncher: ActivityResultLauncher<Array<String>>
    private lateinit var treePrettyPrintView: TextView
    private lateinit var spinner: Spinner
    private lateinit var adapter: ArrayAdapter<String>
    private lateinit var selectedTypeOption: String

    private lateinit var treeProvider: TreeProvider
    private lateinit var tree: BinaryTree<CustomTypeInterface>
    private var treeAsFileString = ""

    private var programmingSetSpinnerFlag: Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {

        treeProvider = TreeProvider()
        val treeProviderOptionTypes = treeProvider.getAvailableTypes()
        selectedTypeOption = treeProviderOptionTypes.first()
        treeProvider.initializeWithEmptyTree(selectedTypeOption)
        tree = treeProvider.getTree() ?: throw NoSuchElementException("No BinaryTree available")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        spinner = findViewById(R.id.spinner)
        adapter =
            ArrayAdapter(this, android.R.layout.simple_spinner_item, treeProviderOptionTypes)
        spinner.adapter = adapter
        val insertRandomButton: Button = findViewById(R.id.insertRandomButton)
        val rebalanceButton: Button = findViewById(R.id.rebalanceButton)
        val elementRepresentationExample: TextView = findViewById(R.id.elementRepresentationExample)
        elementRepresentationExample.text = treeProvider.getObjectStringRepresentationExample()
        treePrettyPrintView = findViewById(R.id.treePrettyPrintView)
        fun updateTreeView() {
            treePrettyPrintView.text = tree.prettyPrintToString()
        }
        val inputDataField: EditText = findViewById(R.id.inputDataField)
        val addElementButton: Button = findViewById(R.id.addElementButton)
        saveToFileButton = findViewById(R.id.saveToFileButton)
        loadFromFileButton = findViewById(R.id.loadFromFileButton)

        createFileLauncher =
            registerForActivityResult(ActivityResultContracts.CreateDocument()) { uri: Uri? ->
                uri?.let {
                    saveTreeToFile(it)
                }
            }

        openFileLauncher =
            registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri: Uri? ->
                uri?.let {
                    loadTreeFromFile(it)
                }
            }

        spinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>,
                view: View?,
                position: Int,
                id: Long
            ) {
                if (programmingSetSpinnerFlag) {
                    programmingSetSpinnerFlag = false
                    return
                }
                val selectedItem = parent.getItemAtPosition(position).toString()
                selectedTypeOption = selectedItem
                treeProvider.initializeWithEmptyTree(selectedTypeOption)
                elementRepresentationExample.text = treeProvider.getObjectStringRepresentationExample()
                tree = treeProvider.getTree()
                    ?: throw NoSuchElementException("No BinaryTree available")
                updateTreeView()
            }

            override fun onNothingSelected(parent: AdapterView<*>) {}
        }

        insertRandomButton.setOnClickListener {
            treeProvider.insertRandomValue()
            updateTreeView()
        }

        rebalanceButton.setOnClickListener {
            tree.rebalance()
            updateTreeView()
        }

        addElementButton.setOnClickListener {
            treeProvider.insertValue(selectedTypeOption, inputDataField.text.toString())
            updateTreeView()
        }

        saveToFileButton.setOnClickListener {
            treeAsFileString = treeProvider.getTreeStringRepresentation() ?: ""
            createFileLauncher.launch("tree.json")
        }

        loadFromFileButton.setOnClickListener {
            openFileLauncher.launch(arrayOf("application/json", "text/plain"))
        }
    }

    private fun saveTreeToFile(uri: Uri) {
        contentResolver.openOutputStream(uri)?.use { outputStream ->
            BufferedWriter(OutputStreamWriter(outputStream)).use { writer ->
                writer.write(treeAsFileString)
            }
        }
    }

    private fun loadTreeFromFile(uri: Uri) {
        contentResolver.openInputStream(uri)?.use { inputStream ->
            BufferedReader(InputStreamReader(inputStream)).use { reader ->
                val content = reader.readText()
                treeAsFileString = content
                val newType = treeProvider.setTreeFromStringRepresentation(content)
                val index = adapter.getPosition(newType)
                programmingSetSpinnerFlag = true
                spinner.setSelection(index)
                tree = treeProvider.getTree()!!
                treePrettyPrintView.text = tree.prettyPrintToString()
            }
        }
    }
}
