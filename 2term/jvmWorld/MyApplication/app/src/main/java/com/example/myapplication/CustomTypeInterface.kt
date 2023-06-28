package com.example.myapplication

interface CustomTypeInterface {

    val typeName: String

    val typeExampleRepresentation: String

    fun createRandomInstance(): CustomTypeInterface

    override fun toString(): String

    fun serializeToString(): String

    fun deserializeFromString(ss: String): CustomTypeInterface?

    fun compare(other: CustomTypeInterface): Int
}
