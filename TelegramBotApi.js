import TelegramBot from 'node-telegram-bot-api';
import { WebSocketServer } from 'ws';
import fs from 'fs';

const wss = new WebSocketServer({ port: 8080 });
const token = '5652580682:AAGbUQsdAf_917Qt-1MhuhY6QrmPkGveWDM';
const bot = new TelegramBot(token, {polling: true});
const ChatId = '880834935';

wss.on('connection', function connection(ws) {
    console.log('cliente conectado');
    ws.on('message', function message(data) {
        let buf = Buffer.from(data);
        let base64data = buf.toString('base64');
        const buffer = Buffer.from(base64data, 'base64');
        fs.writeFileSync("./imagen/image.jpg",buffer);
        bot.sendMessage(ChatId,'!!!TOCARON EL TIMBRE!!!')
        bot.sendPhoto(ChatId,"./imagen/image.jpg");
    });
    ws.on('close', function close(){
        console.log('cliente desconectado');
    });
});