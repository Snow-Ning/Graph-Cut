# Graph-Cut
这个程序是对图割算法的分析与实现。运行此程序需要opencv的支持，我在项目中使用的是opencv2.14的版本，更新的版本应该也可以。
整个程序分为三个文件，分别是源、mystruct、maxFlowAndMinCut文件。
其中源文件用于读取图片、标记对象和背景，mystruct用于将图片转换成网络流图，maxFlowAndMinCut实现了最大流最小割算法。

程序运行后首先会加载设定好的图片，加载完毕后按住左键移动标记对象，按住右键移动标记背景。
标记完成后，按esc键会出现该图片的灰度图片，在按一次esc键会运行最大流最小割算法。运行完毕后会出现图片分割结果。

算法分析与实现效果：https://pan.baidu.com/s/1ERcyABX9EFNuDrLNb9xavA
