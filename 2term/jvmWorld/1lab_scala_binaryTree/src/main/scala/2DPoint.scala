import scala.math.{sqrt, pow}

class Point(val x: Double, val y: Double) {
  override def toString: String = s"X: $x, Y: $y"

  def distanceFromOrigin: Double = sqrt(pow(x, 2) + pow(y, 2))
}

class PointDistanceComparator extends Ordering[Point] {
  override def compare(a: Point, b: Point): Int = {
    val distanceA = a.distanceFromOrigin
    val distanceB = b.distanceFromOrigin

    if (distanceA == distanceB) {
      0
    } else if (distanceA < distanceB) {
      -1
    } else {
      1
    }
  }

  override def equiv(x: Point, y: Point): Boolean = {
    compare(x, y) == 0
  }

  override def lt(x: Point, y: Point): Boolean = {
    compare(x, y) == -1
  }

  override def gt(x: Point, y: Point): Boolean = {
    compare(x, y) == 1
  }
}