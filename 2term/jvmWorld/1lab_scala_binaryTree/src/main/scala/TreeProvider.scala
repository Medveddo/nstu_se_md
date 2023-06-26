
class TreeProvider(var tree: Option[BinaryTree[CustomTypeTrait]] = None) {
  private val deserializers: Map[String, CustomTypeTrait] = Map(
    "GeoCoordinate" -> new GeoCoordinate(0, 0),
    "Point2D" -> new Point2D(0, 0)
  )
  private var currentType: String = ""

  def getAvailableTypes(): List[String] = {
    deserializers.keys.toList
  }

  def getObjectStringRepresentationExample(): String = {
    deserializers.get(currentType) match {
      case Some(deserializer) => deserializer.typeExampleRepresentation
      case None => "Unable to get type representation"
    }
  }

  def insertRandomValue(): Unit = {
    deserializers.get(currentType) match {
      case Some(deserializer) =>
        tree match {
          case Some(binaryTree) => binaryTree.insert(deserializer.createRandomInstance())
          case None => throw new IllegalStateException("Tree not initialized")
        }
      case None => throw new IllegalStateException("Could not find requested type")
    }
  }

  def insertValue(typeName: String, serializedData: String): Unit = {
    deserializers.get(typeName) match {
      case Some(deserializer) =>
        deserializer.deserializeFromString(serializedData) match {
          case Some(value) =>
            tree match {
              case Some(binaryTree) => binaryTree.insert(value.asInstanceOf[CustomTypeTrait])
              case None => throw new IllegalStateException("Tree not initialized")
            }
          case None => throw new IllegalArgumentException(s"Could not deserialize data: $serializedData")
        }
      case None => throw new IllegalArgumentException(s"Unsupported type: $typeName")
    }
  }

  def initializeWithEmptyTree(typeName: String): Unit = {
    deserializers.get(typeName) match {
      case Some(_) =>
        tree = Some(new BinaryTree[CustomTypeTrait])
      case None => throw new IllegalArgumentException(s"Unsupported type: $typeName")
    }

    currentType = typeName
  }


  def getTree(): Option[BinaryTree[CustomTypeTrait]] = tree
}