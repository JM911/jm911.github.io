---
title: "Blog"
layout: archive
permalink: categories/blog
author_profile: true
sidebar_main: true
---


{% assign posts = site.categories.['blog'] %}
{% for post in posts %} {% include archive-single-category.html type=page.entries_layout %} {% endfor %}