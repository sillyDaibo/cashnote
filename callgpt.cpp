#include"callgpt.h"

NetWork::~NetWork()
{
    if(reply!=nullptr){
        reply->deleteLater();
        delete reply;
    }
    delete manager;
}

NetWork::NetWork()
{
    //初始化对象
    APIKey = "0Pplt90cjGGGz7hyyio7dxDT";
    APISecret = "cnXIz20XoVw5JEqP5B4avdacuSO5U3K5";
    accessToken = "24.99a5d6598e784cfd0235e1118c2096d7.2592000.1721657309.282335-85857014";
    manager = new QNetworkAccessManager(this);
}

/*  获取Accesstoken
 *  返回值：int(错误码，-1为异常，0为错误，1为无误)
*/
int NetWork::handle_GetAccessToken()
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        //修改正则表达式
        Regex.setPattern("\"access_token\":\"([^\"]+)\"");

        //在JSON字符串中进行匹配
        QRegularExpressionMatch match = Regex.match(responseData);

        // 如果找到匹配项，则提取access_token数据
        if (match.hasMatch()) {
            accessToken = match.captured(1);
            QMessageBox::warning(nullptr,"提示","设置Accesstoken成功",QMessageBox::Ok);
            return 1;
        }
    }
    else {
        accessToken = " ";      //Token置为空
        QMessageBox::warning(nullptr,"错误",reply->errorString(),QMessageBox::Ok);
        return 0;
    }

    return 0;
}

/*  设置鉴权元素
 *  返回值：int(错误码，-1为异常，0为错误，1为无误)
*/
int NetWork::handle_SetAuthentication(const QString &key, const QString &secret)
{
    //更新APIKey和APISecret
    APIKey = key;
    APISecret = secret;

    //初始化POST请求的参数
    QUrlQuery params;
    params.addQueryItem("grant_type", "client_credentials");
    params.addQueryItem("client_id", APIKey);
    params.addQueryItem("client_secret", APISecret);
    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    // 设置URL和请求头部
    QNetworkRequest request;
    request.setUrl(QUrl("https://aip.baidubce.com/oauth/2.0/token"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "application/json");

    //将请求数据作为第二个参数传递给post函数
    reply = manager->post(request, postData);
    connect(reply, &QNetworkReply::readyRead,this,&NetWork::handle_GetAccessToken);
    return 1;
}

/*  修改询问语句
 *  enquiretext：询问语句
 *  返回值：QString&(修改后的询问语句)
*/
QString NetWork::handle_SetEnquireText(const QString& enquiretext)
{
    //清空请求内容和回复语句
    payload = "";

    //准备请求数据
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = enquiretext;

    QJsonArray messagesArray;
    messagesArray.append(messageObject);
    QJsonObject requestBody;
    requestBody["messages"] = messagesArray;

    //将JSON对象转换为字符串
    QJsonDocument doc(requestBody);
    payload = doc.toJson();

    //设置请求头
    QNetworkRequest request;
    QString url = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions_pro?access_token=" + accessToken;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //发送POST请求
    reply = manager->post(request, payload);

    // 等待响应完成
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 读取响应数据
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray jsonData =  reply->readAll();

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse JSON:" << parseError.errorString();
            return " ";
        }

        // 检查 JSON 是否为对象类型
        if (!jsonDoc.isObject()) {
            qDebug() << "JSON data is not an object.";
            return " ";
        }

        // 获取 JSON 对象
        QJsonObject jsonObj = jsonDoc.object();

        // 提取需要的内容，假设 JSON 中有一个名为 "result" 的键，其值为字符串
        if (jsonObj.contains("result") && jsonObj["result"].isString()) {
            QString resultValue = jsonObj["result"].toString();
            qDebug() << "Result value:" << resultValue;
            // 在这里可以对 resultValue 进行进一步处理
            return resultValue;
        } else {
            qDebug() << "JSON object does not contain expected key or value format is not string.";
            return " ";
        }



    } else {
        QMessageBox::warning(nullptr,"错误",reply->errorString(),QMessageBox::Ok);
        return " ";
    }
}