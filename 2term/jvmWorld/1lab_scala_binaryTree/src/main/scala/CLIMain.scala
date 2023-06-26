object CLIMain {

  def main(args: Array[String]): Unit = {
    val binaryTree = BinaryTree.fromList(
      Seq(
        new GeoCoordinate(1, 2),
        new GeoCoordinate(4, 5),
        new GeoCoordinate(5, 6))
    )
    binaryTree.rebalance()
    binaryTree.prettyPrint()
  }
}