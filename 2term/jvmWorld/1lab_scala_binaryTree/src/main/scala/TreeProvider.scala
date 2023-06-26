
class TreeProvider {
  private var tree: Option[BinaryTree[CustomTypeTrait]] = None
  private val deserializers: Map[String, CustomTypeTrait] = Map(
    new GeoCoordinate(0, 0).typeName -> new GeoCoordinate(0, 0),
    new Point2D(0, 0).typeName -> new Point2D(0, 0)
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

  def getTreeStringRepresentation(): Option[String] = {
    tree match {
      case Some(actualTree) => Some(actualTree.serialize())
      case None => None
    }
  }

  def setTreeFromStringRepresentation(s: String): String = {
    val parsed_type = ujson.read(s)("type").str
    deserializers.get(parsed_type) match {
      case Some(deserializer) =>
        tree = Some(BinaryTree.fromJsonString(s, deserializer))
      case None => throw new IllegalArgumentException(s"Unsupported type: $parsed_type")
    }
    currentType = parsed_type
    parsed_type
  }

  def getTree(): Option[BinaryTree[CustomTypeTrait]] = tree
}