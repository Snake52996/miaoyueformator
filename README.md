**这是我第一个真正完成了的,能解决实际问题的程序**

把[喵阅](https://ouo.us/)上下载的漫画整成能直接看的格式然后排序.*喵阅已经关闭服务(据我所知)*

注意源文件目录需要定位在某本漫画的根目录而不是喵阅文件的根目录(正确示例:E:/miaoyue/comic/6147/),其中这里结尾处的斜杠是必须的.

结果目录结尾不要加斜杠,请确保你填写的目录存在且可写,我们不会检查(因为懒233).建议避免使用中文路径.

所有目录的斜杠方向不重要(混着来也没问题),会自动修复的.
 
现在程序可以正确处理中文字符了,同时开始使用多线程,处理速度得到了提升(测试条件下平均提升50%左右(也就是平均消耗时间是之前的一半左右)).

不排除因为多线程带来各种奇奇怪怪的问题的可能性.有时间我也不会继续改进的.

虽然喵阅已经关闭了服务,但近期将基于miaoyueformator将处理喵阅下载文件的功能加入[HFormatter](https://github.com/Snake52996/HFormatter)作为留念.
