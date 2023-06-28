package com.example.myapplication

import net.jemzart.jsonkraken.JsonKraken
import net.jemzart.jsonkraken.JsonValue

class TreeProvider {
    private var tree: BinaryTree<CustomTypeInterface>? = null
    private val deserializers: Map<String, CustomTypeInterface> = mapOf(
        GeoCoordinate(0.0, 0.0).typeName to GeoCoordinate(0.0, 0.0),
        Point2D(0.0, 0.0).typeName to Point2D(0.0, 0.0),
    )
    private var currentType: String = ""

    fun getAvailableTypes(): List<String> {
        return deserializers.keys.toList()
    }

    fun getObjectStringRepresentationExample(): String {
        return deserializers[currentType]?.typeExampleRepresentation
            ?: "Unable to get type representation"
    }

    fun insertRandomValue() {
        val deserializer = deserializers[currentType]
            ?: throw IllegalStateException("Could not find requested type")
        tree?.insert(deserializer.createRandomInstance())
            ?: throw IllegalStateException("Tree not initialized")
    }

    fun insertValue(typeName: String, serializedData: String) {
        val deserializer = deserializers[typeName]
            ?: throw IllegalArgumentException("Unsupported type: $typeName")
        val value = deserializer.deserializeFromString(serializedData)
            ?: throw IllegalArgumentException("Could not deserialize data: $serializedData")
        tree?.insert(value)
            ?: throw IllegalStateException("Tree not initialized")
    }

    fun initializeWithEmptyTree(typeName: String) {
        if (deserializers.containsKey(typeName)) {
            tree = BinaryTree<CustomTypeInterface>()
            currentType = typeName
        } else {
            throw IllegalArgumentException("Unsupported type: $typeName")
        }
    }

    fun getTreeStringRepresentation(): String? {

        return tree?.serialize()
    }

    fun setTreeFromStringRepresentation(s: String): String {
        val parsedType: String = JsonKraken.deserialize<JsonValue>(s)["type"].cast<String>()
        val deserializer = deserializers[parsedType]
            ?: throw IllegalArgumentException("Unsupported type: $parsedType")
        tree = BinaryTree.fromJsonString(s, deserializer)
        currentType = parsedType
        return parsedType
    }

    fun getTree(): BinaryTree<CustomTypeInterface>? {
        return tree
    }
}
