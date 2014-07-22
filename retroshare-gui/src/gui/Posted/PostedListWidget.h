/*
 * Retroshare Posted List
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef POSTED_LIST_WIDGET_H
#define POSTED_LIST_WIDGET_H

#include <QMap>

#include "gui/gxs/GxsMessageFrameWidget.h"
#include "gui/feeds/FeedHolder.h"
#include "util/TokenQueue.h"

class RsPostedGroup;
class RsPostedPost;
class PostedItem;
class UIStateHelper;

namespace Ui {
class PostedListWidget;
}

class PostedListWidget : public GxsMessageFrameWidget, public FeedHolder, public TokenResponse
{
	Q_OBJECT

public:
	PostedListWidget(const RsGxsGroupId &postedId, QWidget *parent = 0);
	~PostedListWidget();

	/* GxsMessageFrameWidget */
	virtual RsGxsGroupId groupId() { return mPostedId; }
	virtual void setGroupId(const RsGxsGroupId &postedId);
	virtual QString groupName(bool withUnreadCount);
	virtual QIcon groupIcon();
	virtual void setAllMessagesRead(bool read);

	/* FeedHolder */
	virtual QScrollArea *getScrollArea();
	virtual void deleteFeedItem(QWidget *item, uint32_t type);
	virtual void openChat(const RsPeerId& peerId);
	virtual void openComments(uint32_t type, const RsGxsGroupId &groupId, const RsGxsMessageId &msgId, const QString &title);

	/* NEW GXS FNS */
	void loadRequest(const TokenQueue *queue, const TokenRequest &req);

protected:
	virtual void updateDisplay(bool complete);

private slots:
	void createNewGxsId();
	void newPost();

	void submitVote(const RsGxsGrpMsgIdPair& msgId, bool up);

	void getRankings();

//	void subscribeTopic();
//	void unsubscribeTopic();

	void showNext();
	void showPrev();

private:
	void processSettings(bool load);
	void updateShowText();

	void clearPosts();

	/*!
	 * Only removes it from layout
	 */
	void shallowClearPosts();

	void loadPost(const RsPostedPost &post);

	void requestGroupData();
	void loadGroupData(const uint32_t &token);
	void insertPostedDetails(const RsPostedGroup &group);

	// subscribe/unsubscribe ack.
//	void acknowledgeSubscribeChange(const uint32_t &token);

	// posts
	void acknowledgePostMsg(const uint32_t &token);
	void loadPostData(const uint32_t &token);
	void requestPosts();

	// votes
	void acknowledgeVoteMsg(const uint32_t& token);
	void loadVoteData(const uint32_t &token);

	// ranking
	//void loadRankings(const uint32_t& token);
	//void applyRanking(const PostedRanking& ranks);
	void applyRanking();

	// update displayed item
	void updateDisplayedItems(const std::vector<RsGxsMessageId>& msgIds);
	void updateCurrentDisplayComplete(const uint32_t& token);

private:
	int	mSortMethod;
	int	mLastSortMethod;
	int	mPostIndex;
	int	mPostShow;

	int mSubscribeFlags;
	RsGxsGroupId mPostedId;
	QString mName;

	QMap<RsGxsMessageId, PostedItem*> mPosts;
	std::list<PostedItem *> mPostList;

	TokenQueue *mPostedQueue;

	//CommentHolder* mCommentHolder;

	UIStateHelper *mStateHelper;

	/* UI - from Designer */
	Ui::PostedListWidget *ui;
};

#endif