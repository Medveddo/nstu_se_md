import scala.math.BigDecimal.RoundingMode
import scala.math.{pow, sqrt}
import scala.util.{Random, Try}


class Point2D(val x: Double, val y: Double) extends CustomTypeTrait {

  override def typeName: String = "2DPoint"

  override def typeExampleRepresentation: String = "0.5|7.8"

  override def createRandomInstance(): this.type = {
    val r = new Random()
    val x = BigDecimal(r.nextDouble() * 20 - 10).setScale(2, RoundingMode.HALF_UP).toDouble
    val y = BigDecimal(r.nextDouble() * 20 - 10).setScale(2, RoundingMode.HALF_UP).toDouble
    new Point2D(x, y).asInstanceOf[this.type]
  }

  override def toString: String = s"X: $x, Y: $y"

  override def serializeToString: String = {
    s"$x|$y"
  }

  override def deserializeFromString(str: String): Option[this.type] = {
    str.split('|') match {
      case Array(xStr, yStr) =>
        try {
          val x = xStr.toDouble
          val y = yStr.toDouble
          val instance = new Point2D(x, y)
          Some(instance.asInstanceOf[this.type])
        } catch {
          case _: NumberFormatException => None
        }
      case _ => None
    }
  }

  def distanceFromOrigin: Double = sqrt(pow(x, 2) + pow(y, 2))

  override def compare(other: CustomTypeTrait): Int = other match {
    case point: Point2D =>
      val distanceA = this.distanceFromOrigin
      val distanceB = point.distanceFromOrigin
      distanceA.compare(distanceB)
    case _ =>
      throw new IllegalArgumentException("Cannot compare Point with a different type")
  }
}
