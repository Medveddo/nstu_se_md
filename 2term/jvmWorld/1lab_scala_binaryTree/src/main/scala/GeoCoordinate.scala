import scala.math.BigDecimal.RoundingMode

class GeoCoordinate(val latitude: Double, val longitude: Double) extends CustomTypeTrait {
  override def typeName: String = "GeoCoordinate"

  override def typeExampleRepresentation: String = "1.5|2.8"

  override def createRandomInstance(): this.type = {
    val r = scala.util.Random
    val latitude = BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
    val longitude = BigDecimal(r.nextDouble() * 200 - 100).setScale(2, RoundingMode.HALF_UP).toDouble
    new GeoCoordinate(latitude, longitude).asInstanceOf[this.type]
  }

  override def toString: String = {
    s"Lat: $latitude, Long: $longitude"
  }

  override def serializeToString: String = {
    s"$latitude|$longitude"
  }

  override def deserializeFromString(str: String): Option[this.type] = {
    str.split('|') match {
      case Array(latStr, lonStr) =>
        try {
          val latitude = latStr.toDouble
          val longitude = lonStr.toDouble
          val instance = new GeoCoordinate(latitude, longitude)
          Some(instance.asInstanceOf[this.type]) // cast here
        } catch {
          case _: NumberFormatException => None
        }
      case _ => None
    }
  }

  override def compare(other: CustomTypeTrait): Int = other match {
    case geo: GeoCoordinate =>
      if (latitude < geo.latitude) -1
      else if (latitude > geo.latitude) 1
      else 0
    case _ => throw new IllegalArgumentException("Cannot compare GeoCoordinate with a different type")
  }
}
