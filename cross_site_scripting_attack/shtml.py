def sanitize_html(raw_text):
	return raw_text.replace("&", "&amp;").replace("\"", "&quot;") \
	              .replace("<", "&lt;").replace(">", "&gt;")

if __name__ == "__main__":
	myinput = raw_input('enter html\n')
	mystr = sanitize_html(myinput)

	print mystr 
