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

ip_lines = data[0].splitlines()
split_line_a = ip_lines[1].split()
#split_line_b = ip_lines[2].split()

ip_type_a = connection_type(split_line_a)
#ip_type_b = connection_type(split_line_b)

ipaddr_a = split_line_a[split_line_a.index('src')+1]
#ipaddr_b = split_line_b[split_line_b.index('src')+1]

my_ip_a = 'Your %s ip is %s' % (ip_type_a, ipaddr_a)
#my_ip_b = 'Your %s ip is %s' % (ip_type_b, ipaddr_b)

msg = MIMEText(my_ip_a)# + '\n' + my_ip_b)
msg['Subject'] = 'IPs For RPi on %s' % today.strftime('%b %d %y')
msg['From'] = gmail_user
msg['To'] = to

smtpserver.sendmail(gmail_user, [to], msg.as_string())
smtpserver.quit()
