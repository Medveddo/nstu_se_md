package com.example.myapplication

import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import androidx.appcompat.app.AppCompatActivity
import android.widget.Toast

import android.widget.ExpandableListAdapter
import android.widget.ExpandableListView


import android.widget.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val spinner: Spinner = findViewById(R.id.spinner)
        val insertRandomButton: Button = findViewById(R.id.insertRandomButton)
        val rebalanceButton: Button = findViewById(R.id.rebalanceButton)
        val elementRepresentationExample: TextView = findViewById(R.id.elementRepresentationExample)
        val inputDataField: EditText = findViewById(R.id.inputDataField)
        val addElementButton: Button = findViewById(R.id.addElementButton)
        val saveToFileButton: Button = findViewById(R.id.saveToFileButton)
        val loadFromFileButton: Button = findViewById(R.id.loadFromFileButton)
        val expandableListView: ExpandableListView = findViewById(R.id.expandableListView)

        // Populate the Spinner with options
        val options = arrayOf("Option 1", "Option 2", "Option 3")
        val adapter = ArrayAdapter(this, android.R.layout.simple_spinner_item, options)
        spinner.adapter = adapter

        // Setup click listeners for buttons
        insertRandomButton.setOnClickListener {
            // Code to insert random element in binary tree
        }

        rebalanceButton.setOnClickListener {
            // Code to rebalance the binary tree
        }

        addElementButton.setOnClickListener {
            // Code to add an element from the input field to the binary tree
        }

        saveToFileButton.setOnClickListener {
            // Code to save the binary tree to a file
        }

        loadFromFileButton.setOnClickListener {
            // Code to load the binary tree from a file
        }

        // Setup the ExpandableListView with groups and children
        val headers = listOf("Root")
        val children = listOf(listOf("Child 1", "Child 2"))
//        val expandableListView: ExpandableListView = findViewById(R.id.expandableListView)

        // Sample data
        val groupData = listOf(
            mapOf("parent" to "Fruits"),
            mapOf("parent" to "Vegetables")
        )

        val childData = listOf(
            listOf(
                mapOf("child" to "Apple"),
                mapOf("child" to "Banana"),
                mapOf("child" to "Cherry")
            ),
            listOf(
                mapOf("child" to "Carrot"),
                mapOf("child" to "Lettuce"),
                mapOf("child" to "Potato")
            )
        )

        // Create an ExpandableListAdapter
        val adapter2: ExpandableListAdapter = SimpleExpandableListAdapter(
            this,
            groupData,
            android.R.layout.simple_expandable_list_item_1,
            arrayOf("parent"),
            intArrayOf(android.R.id.text1),
            childData,
            android.R.layout.simple_expandable_list_item_2,
            arrayOf("child"),
            intArrayOf(android.R.id.text1)
        )

        // Set the adapter to the ExpandableListView
        expandableListView.setAdapter(adapter2)
    }
}
