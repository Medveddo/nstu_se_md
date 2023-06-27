trait CustomTypeTrait {
  def typeName: String

  def typeExampleRepresentation: String

  def createRandomInstance(): this.type

  def toString: String

  def serializeToString: String

  def deserializeFromString(ss: String): Option[this.type]

  def compare(other: CustomTypeTrait): Int
}
