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

    val provider = TreeProvider()
    val type = provider.getAvailableTypes()[0]
    provider.initializeWithEmptyTree(type)
    provider.insertValue(type, "16.78|9.0")
    provider.insertValue(type, "16.78|9.0")
    provider.insertValue(type, "16.78|9.0")
    provider.insertValue(type, "16.78|9.0")
    provider.getTree()?.prettyPrint()
    provider.getTree()?.rebalance()
    provider.getTree()?.prettyPrint()
}