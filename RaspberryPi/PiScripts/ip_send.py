import subprocess
import smtplib
from email.mime.text import MIMEText
import datetime

def connection_type(word_list):
	if 'wlan0' in word_list or 'wlan1' in word_list:
		con_type = 'wifi'
	elif 'eth0' in word_list:
		con_type = 'ethernet'
	else:
		con_type = 'current'
	return con_type


to = 'kopala.david@gmail.com'
gmail_user = 'kopala.david@gmail.com'
gmail_password = 'password'
smtpserver = smtplib.SMTP('smtp.gmail.com', 587)

smtpserver.ehlo()
smtpserver.starttls()
smtpserver.ehlo()
smtpserver.login(gmail_user, gmail_password)
today = datetime.date.today()

arg = 'ip route list'
p = subprocess.Popen(arg, shell=True, stdout=subprocess.PIPE)
data = p.communicate()

ip_lines_un = data[0].splitlines()
ip_lines = []
for i in range(0, len(ip_lines_un)):
	if ip_lines_un[i].split()[0] != 'default':
		ip_lines.append(ip_lines_un[i])

message = ''
for i in range(0, len(ip_lines)):
	split_line = ip_lines[i].split()
	ip_type = connection_type(split_line)
	ipaddr = split_line[split_line.index('src')+1]
	message += '%s: %s\n' % (ip_type, ipaddr)

msg = MIMEText(message)# + '\n' + my_ip_b)
msg['Subject'] = 'IPs For RPi on %s' % today.strftime('%b %d %y')
msg['From'] = gmail_user
msg['To'] = to

smtpserver.sendmail(gmail_user, [to], msg.as_string())
smtpserver.quit()
