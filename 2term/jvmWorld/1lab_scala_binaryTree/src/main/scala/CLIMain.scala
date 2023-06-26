import io.circe._
import io.circe.generic.semiauto._
import java.nio.file._
import scala.io.Source
import java.io._

object CLIMain {

  def main(args: Array[String]): Unit = {
    val binaryTree = BinaryTree.fromList(
      Seq(
        new GeoCoordinate(1, 2),
        new GeoCoordinate(4, 5),
        new GeoCoordinate(5, 6))
    )(new GeoCoordinateLatitudeComparator)
    binaryTree.rebalance()
    binaryTree.prettyPrint()
  }
}