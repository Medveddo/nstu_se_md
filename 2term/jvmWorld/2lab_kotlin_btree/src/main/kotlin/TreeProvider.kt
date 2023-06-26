
class TreeProvider {
    private var tree: BinaryTree<CustomTypeTrait>? = null
    private val deserializers: Map<String, CustomTypeTrait> = mapOf(
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
            tree = BinaryTree<CustomTypeTrait>()
            currentType = typeName
        } else {
            throw IllegalArgumentException("Unsupported type: $typeName")
        }
    }

    fun getTreeStringRepresentation(): String? {

        return tree?.serialize()
    }

//    fun setTreeFromStringRepresentation(s: String): String {
//        val parsedType = Json.plain.parseJson(s).jsonObject["type"]!!.content
//        val deserializer = deserializers[parsedType]
//            ?: throw IllegalArgumentException("Unsupported type: $parsedType")
//        tree = BinaryTree.fromJsonString(s, deserializer)
//        currentType = parsedType
//        return parsedType
//    }

    fun getTree(): BinaryTree<CustomTypeTrait>? {
        return tree
    }
}
