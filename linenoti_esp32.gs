function lineFunction() {
  var token = 'Utz3FSwPvcjRwFaS8FbO4t5IpucOeR33oFgtlP0rEP0'
  var ss = SpreadsheetApp.openById('1TGjEiBIUHMZNtOIe3OyuWjotZ425C5HquOLxiCT3v6Y')
  var sh = ss.getSheetByName('ชีต2')
  var row = sh.getLastRow();

  var today = Utilities.formatDate(new Date(), "GMT+7", "dd/MM/yyyy")
  var time = Utilities.formatDate(new Date(), "GMT+7", "HH:mm")
  //var timer = "06:00"
  //var today = "01/04/2023"
  //Logger.log(today)
  //Logger.log(time)
  //var message = "สวัสดี CNO"
  //sendLineNotify(message, token)

  for (i = 2; i <= row; i++) {
    //var date = Utilities.formatDate(sh.getRange(i,1).getValue(),"GMT+7", "dd/MM/yyyy")
    var temp = sh.getRange(i, 3).getValue() //ดึงข้อมูล row2 column3 เก็บไว้ในตัวแปร temp

    if (temp >= 27) {
      var msg1 = today + '\n'
      var msg2 = time + '\n'
      //var msg1 = DateConvert(sh.getRange(i, 1).getValue()) + '\n'
      //var msg2 = sh.getRange(i, 2).getValue() + '\n'
      var msg3 = sh.getRange(i, 3).getValue() + '\n'
      var msg4 = sh.getRange(i, 4).getValue() + '\n'
      //var msg5 = sh.getRange(i, 5).getValue() + '\n'
      var message = '\n วันที่ : ' + msg1 + ' เวลา : ' + msg2 + ' อุณหภูมิ ˚C : ' + msg3 + ' ความชื้น : ' + msg4
      sendLineNotify(message, token)
      Logger.log(message)
    }
  }
}

 function sendLineNotify(message, token) {
  var options = {
    "method": "post",
    "payload": {
      "message": message,
    },
    "headers": { "Authorization": "Bearer " + token }
  };
  UrlFetchApp.fetch("https://notify-api.line.me/api/notify", options);
} 

function DateConvert(date) {         

    var yyyy = date.getFullYear().toString();
    var mm = (date.getMonth()+1).toString(); // getMonth() is zero-based
    var dd  = date.getDate().toString();

    return (dd[1]?dd:"0"+dd[0]) + '-' + (mm[1]?mm:"0"+mm[0]) + '-' + yyyy;
};