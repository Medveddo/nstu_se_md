class GeoCoordinate(val latitude: Double, val longitude: Double) {
  override def toString: String = {
    s"Lat: $latitude, Long: $longitude"
    //    s"Latitude: $latitude, Longitude: $longitude"
  }
}

class GeoCoordinateLatitudeComparator extends Ordering[GeoCoordinate] {
  override def compare(a: GeoCoordinate, b: GeoCoordinate): Int = {
    if (a.latitude == b.latitude) {
      0
    }
    else if (a.latitude > b.latitude) {
      1
    }
    else {
      -1
    }
  }

  override def equiv(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == 0
  }

  override def lt(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == -1
  }

  override def gt(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == 1
  }
}

class GeoCoordinateLongitudeComparator extends Ordering[GeoCoordinate] {
  override def compare(a: GeoCoordinate, b: GeoCoordinate): Int = {
    if (a.longitude == b.longitude) {
      0
    }
    else if (a.longitude > b.longitude) {
      1
    }
    else {
      -1
    }
  }

  override def equiv(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == 0
  }

  override def lt(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == -1
  }

  override def gt(x: GeoCoordinate, y: GeoCoordinate): Boolean = {
    compare(x, y) == 1
  }
}
