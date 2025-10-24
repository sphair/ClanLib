<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output 
    method="html" 
    indent="yes"
    doctype-public="-//W3C//DTD HTML 4.01//EN" 
    doctype-system="http://www.w3.org/TR/html4/strict.dtd"
    encoding="ISO-8859-1" />

  <xsl:param name="filename"/>
  
  <xsl:template match="node()|@*">
    <xsl:copy><xsl:apply-templates select="@* | node()" /></xsl:copy>
  </xsl:template>

  <xsl:template match="dlink">
    <a href="{@href}"><xsl:value-of select="@href"/></a>
  </xsl:template> 

  <xsl:template match="dlink-full">
    <a href="{@href}"><xsl:value-of select="@href"/></a>
  </xsl:template> 

  <xsl:template match="page">
    <html>
      <head>
        <title>ClanLib - <xsl:value-of select="@title" /></title>
        <link rel="stylesheet" type="text/css" href="default.css" />
        <!--<link REL="help" HREF="faq.html" title="FAQ"/>
        <link REL="section" HREF="faq.html" title="Intro"/>
        <link REL="shortcut icon" href="/favicon.ico" type="image/x-icon" />-->
      </head>
      <body style="background-color: #a4daea; color: black; margin: 1em 3em 1em 3em;">
        <div style="border-style: solid; border-width:thin; border-color: black;">
          <div style="background-color: white; color: black; padding: .3em 1em .3em 1em; border-bottom-style: dotted; border-bottom-width: 2px;">

            <table cellspacing="0" cellpadding="0" border="0" width="100%">
              <tr>
                <td align="center"><h1><a href="http://www.clanlib.org"><img style="border-style: none; padding-right: 130px;" src="../gfx/clanlib.png" alt="ClanLib" /></a></h1></td>
              </tr>
            </table>

            <!-- <xsl:apply-templates select="document('menu.xml')" /> -->

          </div>
          <div style="background-color: white; padding: 1em 3em 1em 3em;">
            <xsl:apply-templates />

            <div style="margin-top: 0em; text-align: center; color: #a0a0a0; border-top-style: dotted; border-top-width: 1px;">
              Questions or comments, write to the <a href="http://clanlib.org/contact.html">ClanLib mailing list</a>.
            </div>
          </div>
        </div>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="menu">
    <div class="menu">
      <xsl:apply-templates />
    </div>
  </xsl:template>

  <xsl:template match="faq-item">
    <div style="border-bottom-style: dotted;  border-bottom-width: 1px; margin-bottom: 1em;">
      <p>
        <table>
          <xsl:apply-templates />
        </table>
      </p>
    </div>
  </xsl:template>
  
  <xsl:template match="faq-item/question">
    <tr>
      <td valign="top">
        <p>Question:</p>
      </td>
      <td>
        <xsl:apply-templates />
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="faq-item/answer">
    <tr>
      <td valign="top">
        <p>Answer:</p>
      </td>
      <td>
        <xsl:apply-templates />
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="menu/item">
    <xsl:choose>
      <xsl:when test="concat($filename, '.html')=@file">
        <a class="active" href="{@file}"><xsl:apply-templates /></a>
      </xsl:when>
      <xsl:otherwise>
        <a href="{@file}"><xsl:apply-templates /></a>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="typewriter">
    <div style="font-family: Sans-Serif; background-color: #dddddd; padding: 1em; margin: 1em;">
      <xsl:apply-templates />
    </div>
  </xsl:template>
  
  <xsl:template match="section">
    <div style="border-bottom-style: dotted;  border-bottom-width: 1px; margin-bottom: 1em;">
      <h2><xsl:value-of select="@title" /></h2>
    </div>
    <xsl:apply-templates />
    <br />
  </xsl:template>

  <xsl:template match="subsection">
    <div style="border-bottom-style: dotted;  border-bottom-width: 1px;">
      <h3><xsl:value-of select="@title" /></h3>
    </div>
    <xsl:apply-templates />
    <br />
  </xsl:template>

  <xsl:template match="subsubsection">
    <div style="border-bottom-style: dotted;  border-bottom-width: 1px;">
      <h4><xsl:value-of select="@title" /></h4>
    </div>
    <xsl:apply-templates />
    <br />
  </xsl:template>
  
  <xsl:template match="screenshots">
    <table summary="" width="100%" border="0" cellpadding="0"  cellspacing="0">
      
      <tr>
        <td align="center">
          <xsl:apply-templates select="item[1]"/>
        </td>
        <td align="center">
          <xsl:apply-templates select="item[2]"/>
        </td>
        <td align="center">
          <xsl:apply-templates select="item[3]"/>
        </td>
      </tr>

      <xsl:if test="count(item)>3">
        <tr>
          <td align="center">
            <xsl:apply-templates select="item[4]"/>
          </td>
          <td align="center">
            <xsl:apply-templates select="item[5]"/>
          </td>
          <td align="center">
            <xsl:apply-templates select="item[6]"/>
          </td>
        </tr>
      </xsl:if>


      <xsl:if test="count(item)>6">
        <tr>
          <td align="center">
            <xsl:apply-templates select="item[7]"/>
          </td>
          <td align="center">
            <xsl:apply-templates select="item[8]"/>
          </td>
          <td align="center">
            <xsl:apply-templates select="item[9]"/>
          </td>
        </tr>
      </xsl:if>

    </table>
  </xsl:template>

  <xsl:template match="screenshots/item">
    <a href="gfx/{@file}.jpg"><img class="screenshot" alt="{@file}" src="gfx/{@file}_small.jpg" title="{.}"/></a>
  </xsl:template>

  <xsl:template match="screenshot-menu">
    <p style="text-align: center;">
      [ 
      <a href="screenshots.html">0.1</a>
      |
      <a href="screenshots-0.0.html">0.0</a>
      ]
    </p>
  </xsl:template>

  <xsl:template match="section-toc">
    <ul>
      <xsl:for-each select="section">
        <li><a href="#section{generate-id(.)}">
            <xsl:apply-templates/></a></li>
      </xsl:for-each>
    </ul>
    <hr/>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="faq-list">
    <ul>
      <xsl:for-each select="faq/question">
        <li><a href="#faq{generate-id(.)}">
            <xsl:apply-templates/></a></li>
      </xsl:for-each>
    </ul>
    <hr/>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="faq">
    <p></p>
    <table width="100%"  class="question">
      <colgroup width="60%" />
      <tr><td valign="top">
          <div id="faq{generate-id(question)}">
            <xsl:apply-templates select="question/node()"/>
          </div>
        </td>
        
        
        <td align="right" valign="top">
          <small>Last update:<xsl:value-of select="@date"/></small>
          [<small><a href="#faqtoc">Up</a></small>]
        </td>
      </tr>
    </table>

    <div class="answer"><xsl:apply-templates select="answer/node()"/></div>
  </xsl:template>

  <xsl:template match="news">
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="news/item">
    <div style="margin-bottom: 2em; border-width: 1px; border-style: solid;">
      <div style="background-color: #a4daea; border-bottom-style: solid; border-bottom-width: 1px; font-family: Sans-Serif; padding-left: 10px;">
        <xsl:value-of select="@date"/>: 
        <xsl:value-of select="title" />
      </div>
      <div style="padding-left: 1em; padding-right: 1em;">
        <xsl:apply-templates select="*[position()>1]" />
      </div>
    </div>
  </xsl:template>

  <xsl:template match="games">
    <table align="center" width="70%">
      <xsl:apply-templates select="game">
        <xsl:sort select="name" />
      </xsl:apply-templates>
    </table>
  </xsl:template>

  <xsl:template match="game">
    <tr>
      <td valign="middle">
        <br />
        <xsl:choose>
          <xsl:when test="image != ''">
            <a href="http://clanlib.org/~sphair/gfx/{image}.jpg"><img src="http://clanlib.org/~sphair/gfx/{image}_small.jpg" alt="{name} Screenshots" /></a>
          </xsl:when>
          <xsl:otherwise>
            <img src="gfx/noscreenshot.png" alt="" />
          </xsl:otherwise>
        </xsl:choose>
        <br />
      </td>
      
      <td>
        <strong><xsl:value-of select="name" /> 
          <xsl:if test="url != ''">
            - <a href="{url}"><xsl:value-of select="url" /></a>
          </xsl:if>
        </strong><br />
        <xsl:value-of select="description" />
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="tm">
    <sup>tm</sup>
  </xsl:template>

  <xsl:template match="thumbnail-link">
    <a href="{@href}"><img 
        class="screenshot" 
        src="{substring-before(@href, '.jpg')}_small.jpg"
        alt="{substring-before(@href, '.jpg')}_small.jpg"/></a>
  </xsl:template>
</xsl:stylesheet>
