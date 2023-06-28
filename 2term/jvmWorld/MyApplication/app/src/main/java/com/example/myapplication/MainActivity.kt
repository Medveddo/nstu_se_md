package com.example.myapplication

import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import androidx.appcompat.app.AppCompatActivity
import android.widget.Toast

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val spinner: Spinner = findViewById(R.id.spinner)
        val button1: Button = findViewById(R.id.button1)
        val button2: Button = findViewById(R.id.button2)

        // Populate Spinner with items
        val items = arrayOf("Option 1", "Option 2", "Option 3")
        val adapter = ArrayAdapter(this, android.R.layout.simple_spinner_item, items)
        spinner.adapter = adapter

        // Set an on item selected listener for spinner
        spinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(parent: AdapterView<*>, view: View, position: Int, id: Long) {
                val item = parent.getItemAtPosition(position).toString()
                Toast.makeText(this@MainActivity, "Selected: $item", Toast.LENGTH_SHORT).show()
            }

            override fun onNothingSelected(parent: AdapterView<*>) {}
        }

        // Set click listener for the buttons
        button1.setOnClickListener {
            Toast.makeText(this, "Button 1 clicked", Toast.LENGTH_SHORT).show()
        }

        button2.setOnClickListener {
            Toast.makeText(this, "Button 2 clicked", Toast.LENGTH_SHORT).show()
        }
    }
}
