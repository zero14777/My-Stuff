<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
<html>
<body>
<h2>My Game List</h2>
<table border="1">
<tr bgcolor="#9acd32">
<th>Year</th>
<th>Game</th>
<th>Description</th>
</tr>
<xsl:for-each select="Games/Computer">
<tr>
<td><xsl:value-of select="year"/></td>
<td><xsl:value-of select="Game"/></td>
<td><xsl:value-of select="action"/></td>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>