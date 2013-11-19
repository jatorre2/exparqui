require 'json'
require 'net/http'

require 'serialport'


sp = SerialPort.new "/dev/ttyAMA0", 9600

sp.data_bits = 8
sp.stop_bits = 1
sp.parity = SerialPort::NONE

head = "\r" + "="*17 + "   ESPN - News   " + "="*17 

while true 
	res = Net::HTTP.get_response URI.parse("http://api.espn.com/v1/now/?leagues=tennis&apikey=[???]")

	ob = JSON.parse(res.body)

	news = ob["feed"].sample
	headline = news["headline"]
	description = news["description"]

	sleep(1.0/100.0)

	sp.write "\b" + head + headline + "\r\n"+"-"*17

	puts headline + "\t:\t" + description 

	sleep 5

	chunks = description.scan(/.{1,#{119}}/)
	chunks.each_with_index do |st,i|
		sp.write "\b#{i+1}/#{chunks.length}\r\n" 
		sp.write st
		sleep 5
	end
end
