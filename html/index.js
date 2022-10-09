let express = require("express");
let app = express();
//配置跨域相关
app.use(function (req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header('Access-Control-Allow-Methods', 'PUT, GET, POST, DELETE, OPTIONS');
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header('Access-Control-Allow-Headers', 'Content-Type');
    next();
});
app.use(express.json());
app.listen(9090)

let shell = require('shelljs')
app.post("/", (req, res) => {
    let codeInput = req.body.codeInput
    let commandStr =
        `sudo cat>codeInput.txt <<END
        ${codeInput}`

    shell.exec(commandStr);//输出代码到codeInput.txt文件中
    //调用脚本读取codeInput.txt,并生成五个文本文件
    //分别读取五个文本文件并赋值返回
    let ret1 = codeInput
    let ret2 = codeInput
    let ret3 = codeInput
    let ret4 = codeInput
    let ret5 = codeInput
    res.json({
        ret1, ret2, ret3, ret4, ret5
    });
})