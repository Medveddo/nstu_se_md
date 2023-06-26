//import javafx.application.Application
//import javafx.scene.Scene
//import javafx.scene.control.Label
//import javafx.scene.layout.StackPane
//import javafx.stage.Stage
//
//class HelloWorldApp : Application() {
//    override fun start(primaryStage: Stage) {
//        val message = "Hello World!"
//        val label = Label(message)
//        val root = StackPane(label)
//        val scene = Scene(root, 300.0, 200.0)
//        primaryStage.title = "Hello World GUI"
//        primaryStage.scene = scene
//        primaryStage.show()
//    }
//}

fun main(args: Array<String>) {
//    Application.launch(HelloWorldApp::class.java)

    val values = listOf(GeoCoordinate(0.0,0.0), GeoCoordinate(1.0,0.0),
        GeoCoordinate(-1.0,0.0))
    val tree = BinaryTree.fromList(values)
    tree.prettyPrint()
    val newValue: GeoCoordinate = GeoCoordinate(2.0, -3.0)
    tree.insert(newValue)
}