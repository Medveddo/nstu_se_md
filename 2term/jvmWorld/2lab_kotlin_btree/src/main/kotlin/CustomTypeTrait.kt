interface CustomTypeTrait {

    val typeName: String

    val typeExampleRepresentation: String

    fun createRandomInstance(): CustomTypeTrait

    override fun toString(): String

    fun serializeToString(): String

    fun deserializeFromString(ss: String): CustomTypeTrait?

    fun compare(other: CustomTypeTrait): Int
}
