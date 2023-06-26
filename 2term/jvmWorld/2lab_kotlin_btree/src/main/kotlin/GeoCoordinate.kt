import kotlin.math.round
import kotlin.random.Random

class GeoCoordinate(val latitude: Double, val longitude: Double) : CustomTypeTrait {

    override val typeName: String
        get() = "GeoCoordinate"

    override val typeExampleRepresentation: String
        get() = "1.5|2.8"

    override fun createRandomInstance(): CustomTypeTrait {
        val latitude = round(Random.nextDouble() * 200 - 100 * 100) / 100.0
        val longitude = round(Random.nextDouble() * 200 - 100 * 100) / 100.0
        return GeoCoordinate(latitude, longitude)
    }

    override fun toString(): String {
        return "Lat: $latitude, Long: $longitude"
    }

    override fun serializeToString(): String {
        return "$latitude|$longitude"
    }

    override fun deserializeFromString(str: String): CustomTypeTrait? {
        val parts = str.split('|')
        return when {
            parts.size == 2 -> {
                try {
                    val latitude = parts[0].toDouble()
                    val longitude = parts[1].toDouble()
                    GeoCoordinate(latitude, longitude)
                } catch (e: NumberFormatException) {
                    null
                }
            }
            else -> null
        }
    }

    override fun compare(other: CustomTypeTrait): Int {
        return when (other) {
            is GeoCoordinate -> {
                when {
                    latitude < other.latitude -> -1
                    latitude > other.latitude -> 1
                    else -> 0
                }
            }
            else -> throw IllegalArgumentException("Cannot compare GeoCoordinate with a different type")
        }
    }
}
