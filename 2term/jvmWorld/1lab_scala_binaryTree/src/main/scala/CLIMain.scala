import java.io.{BufferedReader, BufferedWriter, FileReader, FileWriter}

object CLIMain {

  def main(args: Array[String]): Unit = {
    val tree = BinaryTree.fromList(Seq(new GeoCoordinate(1, 2),
      new GeoCoordinate(4, 5),
      new GeoCoordinate(5, 6),
      new GeoCoordinate(-1, -2),
      new GeoCoordinate(-0.5, -5),
      new GeoCoordinate(-7, -2),
      new GeoCoordinate(-5, -2),
      new GeoCoordinate(-8, -2),
    ))
    tree.prettyPrint()
    tree.rebalance()
    tree.prettyPrint()

    val serializedTree = tree.serialize()

    val file = new BufferedWriter(new FileWriter("tree.json"))
    file.write(serializedTree)
    file.close()

    val fileReader = new BufferedReader(new FileReader("tree.json"))
    val jsonString = fileReader.readLine()
    fileReader.close()

    val reconstructedTree = BinaryTree.fromJsonString(jsonString, new GeoCoordinate(0, 0))
    reconstructedTree.prettyPrint()
  }
}