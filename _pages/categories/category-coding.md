---
title: "Coding"
layout: archive
permalink: categories/coding
author_profile: true
sidebar_main: true
---


{% assign posts = site.categories.['coding'] %}
{% for post in posts %} {% include archive-single-category.html type=page.entries_layout %} {% endfor %}