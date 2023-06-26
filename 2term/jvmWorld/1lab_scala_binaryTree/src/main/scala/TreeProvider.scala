import scala.math.BigDecimal.RoundingMode

object DefaultTypeAliases {
  val IntTypeStrName = "Int"
}

class TreeProvider(var tree: Option[BinaryTree[_]] = None, var treeObjectType: String = "") {
  def getAvailableTypes(): List[String] = {
    List(DefaultTypeAliases.IntTypeStrName, GeoCoordinate.strname, Point.strname)
  }

  def getObjectStringRepresentationExample(): String = {
    treeObjectType match {
      case GeoCoordinate.strname => GeoCoordinate.strreprexample
      case Point.strname => Point.strreprexample
      case DefaultTypeAliases.IntTypeStrName => "42"
      case _ => "Unknown type"
    }
  }

  def initializeWithEmptyTree(objectType: String = DefaultTypeAliases.IntTypeStrName): Unit = {
    if (objectType == GeoCoordinate.strname) {
      treeObjectType = GeoCoordinate.strname
      tree = Some(BinaryTree.fromList(Seq.empty[GeoCoordinate])(new GeoCoordinateLatitudeComparator))
    }
    else if (objectType == Point.strname) {
      treeObjectType = Point.strname
      tree = Some(BinaryTree.fromList(Seq.empty[Point])(new PointDistanceComparator))
    }
    else if (objectType == DefaultTypeAliases.IntTypeStrName) {
      treeObjectType = DefaultTypeAliases.IntTypeStrName
      tree = Some(BinaryTree.fromList(Seq.empty[Int])(new DefaultComparator[Int]))
    }
  }

  def insertRandomValue(objectType: String): Unit = {
    val realTree: BinaryTree[_] = tree match {
      case Some(actualTree) => actualTree
      case None => return
    }
    val r = scala.util.Random
    objectType match {
      case DefaultTypeAliases.IntTypeStrName => realTree.asInstanceOf[BinaryTree[Int]].insert(r.nextInt())
      case Point.strname => realTree.asInstanceOf[BinaryTree[Point]].insert(
        new Point(
          BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble,
          BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
        )
      )

      case GeoCoordinate.strname => realTree.asInstanceOf[BinaryTree[GeoCoordinate]].insert(
        new GeoCoordinate(
          BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble,
          BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
        )
      )
    }
  }

  def insertValue(objectType: String, insertedValue: String): Unit = {
    var realTree: BinaryTree[_] = tree match {
      case Some(actualTree) => actualTree
      case None => return
    }
    objectType match {
      case DefaultTypeAliases.IntTypeStrName =>
        def parseInt(str: String): Option[Int] = {
          try {
            Some(str.toInt)
          } catch {
            case _: NumberFormatException => None
          }
        }
        val intValue = parseInt(insertedValue)
        intValue match {
          case Some(realIntValue) => realTree.asInstanceOf[BinaryTree[Int]].insert(realIntValue)
          case None => println("Failed to parse integer")
        }
      case Point.strname =>
        val pointValue = Point.deserializeFromString(insertedValue)
        pointValue match {
          case Some(realPointValue) => realTree.asInstanceOf[BinaryTree[Point]].insert(realPointValue)
          case None => println("Failed to parse Point")
        }
      case GeoCoordinate.strname =>
        val geoValue = GeoCoordinate.deserializeFromString(insertedValue)
        geoValue match {
          case Some(realGeoValue) => realTree.asInstanceOf[BinaryTree[GeoCoordinate]].insert(realGeoValue)
          case None => println("Failed to parse GeoCoordinate")
        }
    }
  }

  def getTree(): Option[BinaryTree[_]] = tree
}

